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


// Initialization of static fields
std::shared_ptr<OperatorApp> OperatorApp::instance{nullptr};
BS::thread_pool OperatorApp::pool{};



std::shared_ptr<OperatorApp> OperatorApp::GetInstance() {
    if(instance == nullptr){
        instance = std::shared_ptr<OperatorApp>(new OperatorApp());
        instance->modulesManager->registerModule(instance);
    }
    return instance;
}


OperatorApp::OperatorApp() : TaskGenerator({"system-state", "0.0.0"}){


    //ui = new CoreControlWidget(instance, mainWindow);

    // setup services
    connectionsManager = std::make_shared<ConnectionsManager>(
            ConnectionConfig("test-operator", "operator", 10));
    tasksManager = std::make_shared<OperatorTasksManager>();

    modulesManager = std::make_shared<OperatorModulesManager>(std::shared_ptr<ITaskHandler>(tasksManager.get()));


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
        emit resultsHandler->onConnectionStateChange(opened, state, time);
    });
}


void OperatorApp::stop() {
    connectionsManager->stop();
    connectResult.get();
}


void OperatorApp::handleResult(Task task) {
    msgs::ActiveAgents agents{};
    agents.ParseFromString(std::get<std::string>(task.payload));

    emit resultsHandler->onActiveAgents(agents);
}


void *OperatorApp::getUI() {
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