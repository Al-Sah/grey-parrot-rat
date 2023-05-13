//
// Created by alsah on 08.05.23.
//

#ifndef GREY_PARROT_BOT_TASKS_MANAGER_H
#define GREY_PARROT_BOT_TASKS_MANAGER_H

#include <string>
#include <queue>
#include <list>
#include <thread>
#include "ITasksRegister.h"
#include "../networking/ControlMessageHeader.h"
#include "../../../modules-sdk/Task.h"
#include "../../../modules-sdk/TaskResult.h"
#include "TasksMap.h"


class TasksManager : public ITasksRegister {

public:
    void addTask(ControlMessageHeader header, std::vector<std::byte> payload) override;

private:

    volatile bool run = true;


    TasksMap tasks;


    Task handleSingleMessage(const ControlMessageHeader& header, std::vector<std::byte> payload);

    std::optional<Task> handleFirstMessage(
            const ControlMessageHeader& header,
            std::vector<std::byte> payload);

    std::optional<Task> handleContinuationMessage(
            const ControlMessageHeader& header,
            std::vector<std::byte> payload);

    Task handleLastMessage(const ControlMessageHeader& header, std::vector<std::byte> payload);


};


#endif //GREY_PARROT_BOT_TASKS_MANAGER_H
