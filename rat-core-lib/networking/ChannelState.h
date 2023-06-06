//
// Created by alsah on 02.04.23.
//

#ifndef GREY_PARROT_RAT_CORE_CHANNEL_STATE_H
#define GREY_PARROT_RAT_CORE_CHANNEL_STATE_H

#include <cstdint>

enum class ChannelState : std::uint16_t {

    /// Channel is initialized, all handlers are set (ready to open)
    INITIALIZED = 1,

    /// Trying to open connection
    OPENING = 2, // RECOVERING

    /// Tmp state (before marked as closed)
    ERROR_CAUGHT = 3,

    /// Connection was established
    OPENED = 4,

    /// After connection was lost and recovering failed wait for some time ...
    WAITING_TO_RECOVER = 5,

    /// Connection was closed
    CLOSED = 6,
};

#endif //GREY_PARROT_RAT_CORE_CHANNEL_STATE_H
