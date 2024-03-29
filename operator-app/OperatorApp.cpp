//
// Created by alsah on 11.03.23.
//

#include "OperatorApp.h"

#include <iostream>
#include <memory>
#include "core-data.pb.h"
#include "networking/ConnectionsManager.h"
#include "tasks-managing/TasksManagerBase.h"
#include "core/OperatorTasksManager.h"

// TODO: tmp solution
#include "../modules/echo/EchoMaker.h"


// Initialization of static fields
std::shared_ptr<OperatorApp> OperatorApp::instance{nullptr};
BS::thread_pool OperatorApp::pool{};



std::shared_ptr<OperatorApp> OperatorApp::GetInstance(QWidget* parent) {
    if(instance == nullptr){
        instance = std::shared_ptr<OperatorApp>(new OperatorApp());

        instance->modulesManager->registerModule(instance);
        instance->modulesManager->registerModule(std::make_shared<EchoMaker>());

    }
    return instance;
}


OperatorApp::OperatorApp() : TaskGenerator({"system-state", "0.0.0"}){


    //ui = new CoreControlWidget(instance, mainWindow);

    // setup services
    connectionsManager = std::make_shared<ConnectionsManager>(
            ConnectionConfig("test-operator", "operator", 10));
    tasksManager = std::make_shared<OperatorTasksManager>();

    modulesManager = std::make_shared<OperatorModulesManager>(
            tasksManager,
            [this](auto moduleInfo){
                return requestDC(moduleInfo);
            });
    modulesManager->setPool(&pool);

    // setup dependencies between services
    //
    // Task 'lifecycle':
    // ConnectionsManager -> TasksManagerBase -> TaskExecutorsManager -> module
    // Task result 'lifecycle':
    // module -> TaskExecutorsManager -> TasksManagerBase -> ConnectionsManager

    connectionsManager->setTasksHandler(std::shared_ptr<IControlPacketHandler>(tasksManager.get()));
    tasksManager->setTaskHandler(std::shared_ptr<ITaskHandler>(modulesManager.get()));
    tasksManager->setMessagesSender(std::shared_ptr<IControlPacketSender>(connectionsManager.get()));

    connectionsManager->setC2StateChangeCallback([this](bool opened, const std::string &state, std::uint64_t time){
        emit resultsHandler->onC2ServerConnectionState({opened, state, time});
    });
    connectionsManager->setPeerStateChangeCallback([this](bool opened, const std::string &state, std::uint64_t time){
        emit resultsHandler->onPeerConnectionState({opened, state, time});
    });

    tasksManager->setOnTasksCountChange([this](int count){
        emit resultsHandler->onRunningTasksCountChange(count);
    });
}


void OperatorApp::stop() {
    connectionsManager->stop();
}


void OperatorApp::handleResult(Task task) {

    msgs::CtrlModuleMessage msg{};
    msg.ParseFromString(std::get<std::string>(task.payload));

    switch (msg.type()) {

        case msgs::CtrlModuleMessage_Type_SET_ALL:
            emit resultsHandler->onActiveAgents(msg.agents());
            break;
        case msgs::CtrlModuleMessage_Type_NEW:
            emit resultsHandler->onNewAgent(msg.agent());
            break;
        case msgs::CtrlModuleMessage_Type_UPDATE:
            emit resultsHandler->onNewAgent(msg.agent());
            break;
        case msgs::CtrlModuleMessage_Type_DISCONNECT:
            emit resultsHandler->onAgentDisconnect(msg.agentid());
            break;
        default:
            std::cout << "OperatorApp: received message with invalid data type";
            break;
    }
}

QWidget *OperatorApp::getUI(QWidget *parent) {
    return nullptr;
}

void OperatorApp::connect(const std::string& c2sever) {
    connectResult = pool.submit([this, c2sever](){
        connectionsManager->start(c2sever);
    });
}

const std::string &OperatorApp::getVersion() const {
    return version;
}

void OperatorApp::setResultsHandler(const std::shared_ptr<CoreCtrlBridge> &resultsHandler) {
    OperatorApp::resultsHandler = resultsHandler;
}

void OperatorApp::requestNotifications() {

    Task task{
        .id = 1,
        .module = this->getModuleInfo().id,
        .payload = "get",
        .isFilepath = false,
        .isPartial = false,
    };

    callback(task);
}

std::vector<ModuleInfo> OperatorApp::getModulesInfo() const {
    return modulesManager->getModulesInfo();
}

std::vector<TaskInfo> OperatorApp::getRunningTasks() const {
    return tasksManager->getTasksMap().getTasks();
}

void OperatorApp::disconnectFromPeer() {
    connectionsManager->disconnectFromCurrentPeer();
}

void OperatorApp::connectToPeer(const std::string &peer) {
    connectionsManager->connectToPeer(peer);
}

std::vector<std::shared_ptr<TaskGenerator>> OperatorApp::getTaskGenerators(const std::vector<std::string>& names) {
    std::vector<std::shared_ptr<TaskGenerator>> res;

    for (const auto &name: names){
        auto modulePrt = modulesManager->getModule(name);
        if(modulePrt != nullptr){
            res.push_back(modulePrt);
        }
    }
    return res;
}


std::shared_ptr<rtc::DataChannel> OperatorApp::requestDC(const ModuleInfo& moduleInfo) {
    return connectionsManager->createDataChannel(moduleInfo);
}

void OperatorApp::setDataChannelHandlers() {

}
