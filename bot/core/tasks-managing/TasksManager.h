//
// Created by alsah on 08.05.23.
//

#ifndef GREY_PARROT_BOT_TASKS_MANAGER_H
#define GREY_PARROT_BOT_TASKS_MANAGER_H

#include <string>
#include <queue>
#include <list>
#include "ITasksRegister.h"
#include "../networking/ControlMessageHeader.h"


enum class TaskState{

    REGISTERED,   // waiting to the free thread to start executing
    ACCUMULATING, // is used if module can not handle partial messages
    EXECUTING,    // task was passed to the execution
    CLOSING       // module requested to close task
};

class Task{

    std::string task_id;
    std::string module_id;
    TaskState state;
};


class TasksManager : public ITasksRegister {

public:
    void addTask(ControlMessageHeader header, std::vector<std::byte> payload) override;

private:

    volatile bool run = true;


    std::list<Task> tasks;

    //std::queue<std::pair<ControlMessageHeader,std::vector<std::byte>>> inboxMessages;
};


#endif //GREY_PARROT_BOT_TASKS_MANAGER_H
