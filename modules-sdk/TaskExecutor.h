//
// Created by alsah on 10.05.23.
//

#ifndef GREY_PARROT_RAT_SDK_TASK_EXECUTOR_H
#define GREY_PARROT_RAT_SDK_TASK_EXECUTOR_H

#include <utility>
#include <functional>

#include "Task.h"
#include "ModuleInfo.h"

class TaskExecutor{

public:
    virtual void execute(Task task) = 0;

    [[nodiscard]] ModuleInfo getModuleInfo() const;

    explicit TaskExecutor(ModuleInfo aModuleInfo);
    TaskExecutor( ModuleInfo aModuleInfo, const std::function<void(Task)>& callback);

    void setCallback(const std::function<void(Task)> &aCallback);

protected:
    const ModuleInfo moduleInfo;
    std::function<void(Task)> callback;
};


#endif //GREY_PARROT_SDK_TASK_EXECUTOR_H
