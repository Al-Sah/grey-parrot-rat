//
// Created by alsah on 11.03.23.
//

#include "Bot.h"
#include "bot-version.h"

#include <iostream>
#include <csignal>
#include <fstream>
#include <sstream>
#include <sys/utsname.h>


// Initialization of static fields
Bot* Bot::bot = nullptr;
volatile bool Bot::run = true;


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
    Bot::run = false;
}

Bot::Bot() :
    botVersion(GREY_PARROT_BOT_VERSION),
    computerId(retrieveComputerId()){

    // Register signal handlers
    std::signal(SIGINT, handleSystemSignal);
    std::signal(SIGTERM, handleSystemSignal);
}

Bot *Bot::GetInstance() {
    if(bot == nullptr){
        bot = new Bot();
    }
    return bot;
}

int Bot::runPerpetual() {
    while (Bot::run){
        sleep(1);
    }
    return 0;
}


std::string Bot::retrieveComputerId() {
    utsname unameData{};
    uname(&unameData);

    std::stringstream ss;
    ss << unameData.sysname << unameData.nodename << unameData.machine;

    std::ifstream cpuinfo ("/proc/cpuinfo");
    if (cpuinfo.is_open()) {

        // read just first 4 lines of cpuinfo (starting from second); it contains:
        // vendor_id, cpu family, model, model name
        std::string fileLine;
        for (int lines = 1; lines < 5 && std::getline(cpuinfo, fileLine); ++lines){
            ss << fileLine;
        }
    }
    // TODO get serial number ? (Just right now it is probably enough to use nodename + cpuinfo )
    return "bot-" + std::to_string(std::hash<std::string>()(ss.str()));
}
