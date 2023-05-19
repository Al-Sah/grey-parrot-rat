//
// Created by alsah on 11.03.23.
//

#include "Bot.h"

#include <iostream>
#include <csignal>
#include <thread>
#include <utility>

#if defined(PLATFORM_IS_LINUX)
    #include "platform-info/collectors/LinuxInfoCollector.h"
    #define GENERATE_BOT_INSTANCE()new Bot(std::unique_ptr<IDeviceDetailsCollector>(new LinuxInfoCollector()))
#elif defined(PLATFORM_IS_WINDOWS)
    #include "platform-info/collectors/WinInfoCollector.h"
    #define GENERATE_BOT_INSTANCE()new Bot(std::unique_ptr<IDeviceDetailsCollector>(new WinInfoCollector()))
#else
    #error "platform is not supported"
#endif

using Json = nlohmann::json;

std::vector<std::byte> getBytes(std::string const &s)
{
    std::vector<std::byte> bytes;
    bytes.reserve(s.size());

    std::transform(std::begin(s), std::end(s), std::back_inserter(bytes), [](char c){
        return std::byte(c);
    });

    return bytes;
}


// Initialization of static fields
Bot* Bot::bot = nullptr;
volatile bool Bot::run = true;
std::mutex Bot::mutex{};
std::condition_variable Bot::run_cv{};
BS::thread_pool Bot::pool{};


void Bot::handleSystemSignal(int signal) {
    switch (signal) {
        case SIGINT:
        case SIGTERM:
            std::cout << "Received signal " << signal << ", shutting down" << std::endl;
            break;
        default: // probably unreachable code
            std::cout << "Unexpectedly received signal " << signal << std::endl;
            break;
    }
    std::lock_guard<std::mutex> lock(Bot::mutex);
    std::cout << "\nsending notification\b";
    Bot::run = false;
    Bot::run_cv.notify_one();
}

// TODO: make shared ??
Bot *Bot::GetInstance() {
    if(bot == nullptr){
        bot = GENERATE_BOT_INSTANCE();
    }
    return bot;
}

int Bot::runPerpetual() {

    auto instance = Bot::GetInstance();

    instance->modulesManager->registerModule(std::shared_ptr<TaskExecutor>(instance));

    auto result = pool.submit([](){bot->connectionsManager->start();});

    while (Bot::run){
        std::unique_lock<std::mutex> lock(Bot::mutex);
        Bot::run_cv.wait_for( lock,std::chrono::seconds(1));
    }
    std::cout << "exited main loop, stopping services\n";

    instance->connectionsManager->stop();
    result.get();

    return 0;
}


Bot::Bot(std::unique_ptr<IDeviceDetailsCollector> infoCollector) :
    TaskExecutor(ModuleInfo("bot-control", "0.0.0")),
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
            ConnectionsManagerConfiguration(deviceDetails.computerId)
            );
    tasksManager = std::make_shared<TasksManager>();
    modulesManager = std::make_shared<ModulesManager>();


    // setup dependencies between services
    //
    // Task 'lifecycle':
    // ConnectionsManager -> TasksManager -> ModulesManager -> module
    // Task result 'lifecycle':
    // module -> ModulesManager -> TasksManager -> ConnectionsManager

    connectionsManager->setTasksHandler(std::shared_ptr<ITasksRegister>(tasksManager.get()));
    tasksManager->setTaskDelegator(std::shared_ptr<ITaskDelegator>(modulesManager.get()));
    tasksManager->setMessagesSender(std::shared_ptr<IMessagesSender>(connectionsManager.get()));


    setCallback([this](TaskResult tr){
        tasksManager->handle(std::move(tr));
    });
}

void Bot::execute(Task task) {

    if(! task.payload.has_value()){
        return;
    }

    // convert std::vector<std::byte> to std::string
    auto str_payload = std::string(
            reinterpret_cast<const char*>(task.payload.value().data()),
            task.payload.value().size()
            );

    if(str_payload != "get-state"){
        std::cout << "Bot: Undefined command";
        return;
    }

    Json json{};

    to_json(json, *this);
    std::string result = to_string(json);

    std::cout << result;

    TaskResult tr{
        .isClosing = true,
        .module_id = moduleInfo.id,
        .task_id = task.task_id,
        .payload = getBytes(result)
    };

    callback(tr);
}


void Bot::to_json(Json &aJson, const Bot &aBot) {

    Json modules_as_json {};
    for (const auto &item: aBot.modulesManager->getModulesInfo()){
        modules_as_json.push_back(item.id + ' ' + item.version);
    }
    aJson = nlohmann::json{
        "app", {
            { "isRoot", aBot.applicationDetails.isRoot },
            { "startTime", aBot.applicationDetails.startTime },
            { "version", aBot.applicationDetails.botVersion },
            { "c4platform", aBot.applicationDetails.compiledFor },
            { "c4arc", aBot.applicationDetails.compiledForArc }
        },
        "device", {
            { "id", aBot.deviceDetails.computerId },
            { "name", aBot.deviceDetails.computerName },
            { "os", aBot.deviceDetails.osFullName }
        },
        "modules", modules_as_json
    };
}
