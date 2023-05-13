//
// Created by alsah on 13.05.23.
//

#ifndef GREY_PARROT_BOT_TASK_DELEGATOR_INTERFACE_H
#define GREY_PARROT_BOT_TASK_DELEGATOR_INTERFACE_H

#include "../../../modules-sdk/Task.h"

/** @interface ITaskDelegator
 *
 * ITaskDelegator delegates task to the module.
 *
 * This interface must be implemented by @class ModulesManager
 */

class ITaskDelegator {

public:
    virtual void delegate(Task task) = 0;
};


#endif //GREY_PARROT_BOT_TASK_DELEGATOR_INTERFACE_H
