//
// Created by alsah on 14.05.23.
//

#ifndef GREY_PARROT_BOT_MESSAGES_SENDER_INTERFACE_H
#define GREY_PARROT_BOT_MESSAGES_SENDER_INTERFACE_H


#include <vector>


class IMessagesSender {
public:
    virtual void sendResult(std::vector<std::byte> payload) = 0;
};


#endif //GREY_PARROT_BOT_MESSAGES_SENDER_INTERFACE_H
