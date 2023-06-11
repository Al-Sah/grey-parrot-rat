//
// Created by alsah on 10.06.23.
//

#ifndef GREY_PARROT_RAT_ECHO_MODULE_ECHO_HANDLER_H
#define GREY_PARROT_RAT_ECHO_MODULE_ECHO_HANDLER_H


#include "TaskExecutor.h"

class EchoHandler : public TaskExecutor{

public:
    EchoHandler();
    void execute(Task task) override;

private:
    void setDataChannelHandlers() override;
};


#endif //GREY_PARROT_RAT_ECHO_MODULE_ECHO_HANDLER_H
