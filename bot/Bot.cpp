//
// Created by alsah on 11.03.23.
//

#include "Bot.h"

#include <iostream>
#include <csignal>
#include <thread>

#if defined(PLATFORM_IS_LINUX)
    #include "platform-info/collectors/LinuxInfoCollector.h"
    #define GENERATE_BOT_INSTANCE()new Bot(std::unique_ptr<IDeviceDetailsCollector>(new LinuxInfoCollector()))
#elif defined(PLATFORM_IS_WINDOWS)
    #include "platform-info/collectors/WinInfoCollector.h"
    #define GENERATE_BOT_INSTANCE()new Bot(std::unique_ptr<IDeviceDetailsCollector>(new WinInfoCollector()))
#else
    #error "platform is not supported"
#endif


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

Bot *Bot::GetInstance() {
    if(bot == nullptr){
        bot = GENERATE_BOT_INSTANCE();
    }
    return bot;
}

int Bot::runPerpetual() {

    Bot::GetInstance();

    auto result = pool.submit([](){bot->connectionsManager->start();});

    while (Bot::run){
        std::unique_lock<std::mutex> lock(Bot::mutex);
        Bot::run_cv.wait_for( lock,std::chrono::seconds(1));
    }
    std::cout << "exited main loop, stopping services\n";

    bot->connectionsManager->stop();
    result.get();

    return 0;
}


Bot::Bot(std::unique_ptr<IDeviceDetailsCollector> infoCollector) :
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

    connectionsManager = std::make_shared<ConnectionsManager>(
            ConnectionsManagerConfiguration(deviceDetails.computerId)
            );

}
