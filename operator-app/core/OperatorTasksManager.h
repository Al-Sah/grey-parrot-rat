//
// Created by alsah on 01.06.23.
//

#ifndef GREY_PARROT_RAT_OPERATOR_CORE_TASKS_MANAGER_H
#define GREY_PARROT_RAT_OPERATOR_CORE_TASKS_MANAGER_H


#include "tasks-managing/TasksManagerBase.h"

class OperatorTasksManager : public TasksManagerBase {
public:
    void handleInbox(msgs::ControlPacket packet) override;

    bool handle(Task data) override;

};


#endif //GREY_PARROT_RAT_OPERATOR_CORE_TASKS_MANAGER_H
