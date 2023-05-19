//
// Created by alsah on 16.03.23.
//

#ifndef GREY_PARROT_BOT_CONNECTIONS_MANAGER_H
#define GREY_PARROT_BOT_CONNECTIONS_MANAGER_H

#include <rtc/rtc.hpp>
#include <memory>

#include "C2ServerChannel.h"
#include "../tasks-managing/ITasksRegister.h"
#include "IMessagesSender.h"


struct ConnectionsManagerConfiguration {

    // bot id is a const computer uuid generated on start
    const std::string &botId;

    // server can be potentially changed in a runtime by request
    std::string c2server;

    explicit ConnectionsManagerConfiguration(const std::string &id) : botId(id) {}
};


class ConnectionsManager : public IMessagesSender{


public:

    explicit ConnectionsManager(ConnectionsManagerConfiguration config);
    ~ConnectionsManager();

    void setTasksHandler(std::shared_ptr<ITasksRegister> tasksRegister);

    ConnectionsManager(ConnectionsManager&) = delete;
    ConnectionsManager(const ConnectionsManager&) = delete;
    void operator = (const ConnectionsManager&) = delete;
    void operator = (ConnectionsManager) = delete;

    void sendResult(std::vector<std::byte> payload) override;

    void start();
    void stop();


private:

    ConnectionsManagerConfiguration config;

    std::shared_ptr<ITasksRegister> tasksRegister;

    std::unique_ptr<C2ServerChannel> c2ServerChannel;

    void handleC2ServerMessage(std::vector<std::byte>& data);

};




#endif //GREY_PARROT_BOT_CONNECTIONS_MANAGER_H
