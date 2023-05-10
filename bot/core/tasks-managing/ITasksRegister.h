//
// Created by alsah on 09.05.23.
//

#ifndef GREY_PARROT_BOT_INBOX_TASKS_REGISTER_INTERFACE
#define GREY_PARROT_BOT_INBOX_TASKS_REGISTER_INTERFACE

#include "../networking/ControlMessageHeader.h"

/** @interface ITasksRegister.
 *
 * ITasksRegister is an interface which is used for registering inbox tasks received from c2server
 *
 * This interface must be implemented by @class TaskManager
 **/


class ITasksRegister {
public:
    /**
    * multithreading-safe method
    * @param1 - raw payload
    * @param2 - payload type (binary or text)
    **/
    virtual void addTask(ControlMessageHeader header, std::vector<std::byte> payload) = 0;
};

#endif //GREY_PARROT_BOT_INBOX_TASKS_REGISTER_INTERFACE