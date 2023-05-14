//
// Created by alsah on 08.05.23.
//

#ifndef GREY_PARROT_BOT_TASKS_MANAGER_H
#define GREY_PARROT_BOT_TASKS_MANAGER_H

#include <string>
#include <queue>
#include <list>
#include <thread>

#include <Task.h>
#include <TaskResult.h>

#include "../networking/ControlMessageHeader.h"
#include "../modules-managing/ITaskDelegator.h"

#include "TasksMap.h"

#include "ITasksRegister.h"
#include "IResultHandler.h"


class TasksManager : public ITasksRegister, public IResultHandler{

public:
    void addTask(ControlMessageHeader header, std::vector<std::byte> payload) override;
    void handle(TaskResult taskResult) override;

    void setTaskDelegator(const std::shared_ptr<ITaskDelegator> &taskDelegator);
private:

    volatile bool run = true;

    TasksMap tasks;

    std::shared_ptr<ITaskDelegator> taskDelegator;


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
