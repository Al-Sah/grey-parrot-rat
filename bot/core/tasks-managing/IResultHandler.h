//
// Created by alsah on 14.05.23.
//

#ifndef GREY_PARROT_BOT_RESULT_HANDLER_INTERFACE_H
#define GREY_PARROT_BOT_RESULT_HANDLER_INTERFACE_H

#include <TaskResult.h>

class IResultHandler {

public:
    virtual void handle(TaskResult taskResult) = 0;
};

#endif //GREY_PARROT_BOT_RESULT_HANDLER_INTERFACE_H
