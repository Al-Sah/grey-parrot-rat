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

    // time in seconds to sleep before recover
    const std::uint16_t sleep;

    // address and port of c2-server; potentially can be changed
    std::string c2server;

    ConnectionConfig(std::string connectionId, std::string endpoint, std::uint16_t sleep);

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

    void setC2StateChangeCallback(
            const std::function<void(bool, std::string, std::uint64_t)> &onC2StateChange);

private:

    ConnectionConfig config;

    std::function<void(bool, std::string, std::uint64_t)> onC2StateChange;

    std::shared_ptr<IControlPacketHandler> packetsHandler;

    std::unique_ptr<C2ServerChannel> c2ServerChannel;

    void handleC2ServerMessage(std::vector<std::byte>& data);

};




#endif //GREY_PARROT_RAT_CORE_CONNECTIONS_MANAGER_H
