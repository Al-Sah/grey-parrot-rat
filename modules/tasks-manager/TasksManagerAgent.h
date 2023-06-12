//
// Created by alsah on 12.06.23.
//

#ifndef GREY_PARROT_RAT_TASKSMANAGERAGENT_H
#define GREY_PARROT_RAT_TASKSMANAGERAGENT_H


#include "TaskExecutor.h"

class ProcessManagerAgent : public TaskExecutor{

public:
    ProcessManagerAgent();
    void execute(Task task) override;

protected:
    void setDataChannelHandlers() override;
};


#endif //GREY_PARROT_RAT_TASKSMANAGERAGENT_H
