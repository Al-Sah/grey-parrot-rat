//
// Created by alsah on 02.06.23.
//

#ifndef GREY_PARROT_RAT_CORE_TASK_HANDLER_INTERFACE_H
#define GREY_PARROT_RAT_CORE_TASK_HANDLER_INTERFACE_H


#include "Task.h"

/**
 * @interface ITaskHandler
 *
 * This interface is used to pass "Task" between core components
 */
class ITaskHandler {
public:
    virtual bool handle(Task data) = 0;
};

#endif //GREY_PARROT_RAT_CORE_TASK_HANDLER_INTERFACE_H
