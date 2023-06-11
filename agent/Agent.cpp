//
// Created by alsah on 11.03.23.
//

#include "Agent.h"

#include <iostream>
#include <csignal>
#include <thread>
#include <utility>

#include <core-data.pb.h>
#include "networking/ConnectionsManager.h"
#include "core/AgentTasksManager.h"

// TODO: tmp solution
#include "../modules/echo/EchoHandler.h"

#if defined(PLATFORM_IS_LINUX)
    #include "platform-info/collectors/LinuxInfoCollector.h"
    #define GENERATE_BOT_INSTANCE()new Agent(std::unique_ptr<IDeviceDetailsCollector>(new LinuxInfoCollector()))
#elif defined(PLATFORM_IS_WINDOWS)
    #include "platform-info/collectors/WinInfoCollector.h"
    #define GENERATE_BOT_INSTANCE()new Agent(std::unique_ptr<IDeviceDetailsCollector>(new WinInfoCollector()))
#else
    #error "platform is not supported"
#endif


// Initialization of static fields
Agent* Agent::bot = nullptr;
volatile bool Agent::run = true;
std::mutex Agent::mutex{};
std::condition_variable Agent::run_cv{};
BS::thread_pool Agent::pool{};


void Agent::handleSystemSignal(int signal) {
    switch (signal) {
        case SIGINT:
        case SIGTERM:
            std::cout << "Received signal " << signal << ", shutting down" << std::endl;
            break;
        default: // probably unreachable code
            std::cout << "Unexpectedly received signal " << signal << std::endl;
            break;
    }
    std::lock_guard<std::mutex> lock(Agent::mutex);
    std::cout << "\nsending notification\b";
    Agent::run = false;
    Agent::run_cv.notify_one();
}

// TODO: make shared ??
Agent *Agent::GetInstance() {
    if(bot == nullptr){
        bot = GENERATE_BOT_INSTANCE();
    }
    return bot;
}

int Agent::runPerpetual() {

    modulesManager->registerModule(std::shared_ptr<TaskExecutor>(this));
    modulesManager->registerModule(std::make_shared<EchoHandler>());

    auto result = pool.submit([this](){
        connectionsManager->start("localhost:8080");
    });

    while (run){
        std::unique_lock<std::mutex> lock(mutex);
        run_cv.wait_for(lock, std::chrono::seconds(1));
    }
    std::cout << "exited main loop, stopping services\n";

    connectionsManager->stop();
    result.get();

    return 0;
}


Agent::Agent(std::unique_ptr<IDeviceDetailsCollector> infoCollector) :
    TaskExecutor(ModuleInfo("agent-control", "0.0.0")),
    applicationDetails(),
    deviceDetails(infoCollector->getDeviceDetails()){

    // Register signal handlers
    std::signal(SIGINT, handleSystemSignal);
    std::signal(SIGTERM, handleSystemSignal);

    // TODO: add logger
    std::cout
            << "isRoot:      " << applicationDetails.isRoot << std::endl
            << "startTime:   " << applicationDetails.startTime << std::endl
            << "Version:     " << applicationDetails.botVersion << std::endl
            << "compiledFor: " << applicationDetails.compiledFor << std::endl
            << "Arc:         " << (std::uint16_t)applicationDetails.compiledForArc << std::endl
            << std::endl
            << "OSFullName: " << deviceDetails.osFullName << std::endl
            << "computerId: " << deviceDetails.computerId << std::endl
            << "computerName: " << deviceDetails.computerName << std::endl
            << std::endl;

    // setup services
    connectionsManager = std::make_shared<ConnectionsManager>(
            ConnectionConfig(deviceDetails.computerId, "agent", 30)
            );

    tasksManager = std::make_shared<AgentTasksManager>();
    modulesManager = std::make_shared<AgentModulesManager>(tasksManager);
    modulesManager->setPool(&pool);

    connectionsManager->setOnDataChannel([this](const std::shared_ptr<rtc::DataChannel>& dc){
        modulesManager->passDataChannel(dc);
    });


    // setup dependencies between services
    //
    // Task 'lifecycle':
    // ConnectionsManager -> TasksManagerBase -> TaskExecutorsManager -> module
    // Task result 'lifecycle':
    // module -> TaskExecutorsManager -> TasksManagerBase -> ConnectionsManager

    connectionsManager->setTasksHandler(tasksManager);
    tasksManager->setTaskHandler(modulesManager);
    tasksManager->setMessagesSender(connectionsManager);


    setCallback([this](Task tr){
        tasksManager->handle(std::move(tr));
    });
}

void Agent::execute(Task task) {

    if(get<std::string>(task.payload) != "get-state"){
        std::cout << "Agent: Undefined command";
        // TODO send notification
        return;
    }

    auto message = to_proto_message();

    Task tr{
        .id = task.id,
        .module = moduleInfo.id,
        .payload = message.SerializeAsString(),
        .isFilepath = false,
        .isClosing = true,
        .isPartial = false
    };

    callback(tr);
}


msgs::AgentDescription Agent::to_proto_message() {

    msgs::AgentDescription agentDescription{};

    msgs::AppInfo *appInfo = agentDescription.mutable_app();
    msgs::DeviceInfo *deviceInfo = agentDescription.mutable_device();

    deviceInfo->set_id(deviceDetails.computerId);
    deviceInfo->set_name(deviceDetails.computerName);
    deviceInfo->set_os(deviceDetails.osFullName);

    appInfo->set_root(applicationDetails.isRoot);
    appInfo->set_start(applicationDetails.startTime);
    appInfo->set_version(applicationDetails.botVersion);
    appInfo->set_c4platform(applicationDetails.compiledFor);
    appInfo->set_c4arc(std::to_string(applicationDetails.compiledForArc));

    for (const auto &item: modulesManager->getModulesInfo()){
        auto moduleInfoPrt = appInfo->add_modules();
        moduleInfoPrt->set_name(item.id);
        moduleInfoPrt->set_version(item.version);
    }
    return agentDescription;
}

void Agent::setDataChannelHandlers() {}
