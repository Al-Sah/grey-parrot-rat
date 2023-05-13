//
// Created by alsah on 10.05.23.
//

#ifndef GREY_PARROT_SDK_TASK_EXECUTOR_INTERFACE_H
#define GREY_PARROT_SDK_TASK_EXECUTOR_INTERFACE_H

#include "Module.h"
#include "Task.h"

class TaskExecutor : public Module{

public:
    virtual void execute(Task task) = 0;
};


#endif //GREY_PARROT_SDK_TASK_EXECUTOR_INTERFACE_H
