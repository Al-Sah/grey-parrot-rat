//
// Created by alsah on 02.04.23.
//

#ifndef GREY_PARROT_BOT_CHANNEL_STATE_H
#define GREY_PARROT_BOT_CHANNEL_STATE_H


#include <cstdint>

enum class ChannelState : std::uint16_t {

    /// All objects are initialized, all handlers are set (ready to open)
    INITIALIZED = 1,

    /// Trying to open connection (just first time)
    TRYING_TO_OPEN = 2, // RECOVERING

    ///
    ERROR_CAUGHT = 3,

    /// Connection was established
    OPENED = 4,

    /// After connection was lost and recovering failed wait for some time ...
    WAITING_TO_RECOVER = 5,


    /// Connection was closed
    CLOSED = 6,

};


#endif //GREY_PARROT_BOT_CHANNEL_STATE_H
