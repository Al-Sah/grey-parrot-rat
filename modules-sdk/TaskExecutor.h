//
// Created by alsah on 10.05.23.
//

#ifndef GREY_PARROT_SDK_TASK_EXECUTOR_INTERFACE_H
#define GREY_PARROT_SDK_TASK_EXECUTOR_INTERFACE_H

#include <utility>
#include <functional>

#include "Task.h"
#include "TaskResult.h"
#include "ModuleInfo.h"

class TaskExecutor{

public:
    virtual void execute(Task task) = 0;

    [[nodiscard]] std::string getModuleId() const;
    [[nodiscard]] std::string getModuleVersion() const;

    explicit TaskExecutor(ModuleInfo aModuleInfo);
    TaskExecutor( ModuleInfo aModuleInfo, const std::function<void(TaskResult)>& callback);

    void setCallback(const std::function<void(TaskResult)> &aCallback);

protected:
    const ModuleInfo moduleInfo;
    std::function<void(TaskResult)> callback;
};


#endif //GREY_PARROT_SDK_TASK_EXECUTOR_INTERFACE_H
