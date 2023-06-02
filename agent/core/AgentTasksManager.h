//
// Created by alsah on 01.06.23.
//

#ifndef GREY_PARROT_AGENT_TASKS_MANAGER_H
#define GREY_PARROT_AGENT_TASKS_MANAGER_H


#include "tasks-managing/TasksManagerBase.h"


class AgentTasksManager : public TasksManagerBase{

public:

    void handleInbox(msgs::ControlPacket packet) override;

    /// handle response from the module
    bool handle(Task data) override;

};


#endif //GREY_PARROT_AGENT_TASKS_MANAGER_H
