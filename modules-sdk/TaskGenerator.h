//
// Created by alsah on 30.05.23.
//

#ifndef GREY_PARROT_RAT_SDK_TASK_GENERATOR_H
#define GREY_PARROT_RAT_SDK_TASK_GENERATOR_H


#include <functional>
#include "Task.h"
#include "ModuleInfo.h"

class TaskGenerator {

public:
    virtual void handleResult(Task task) = 0;

    [[nodiscard]] const ModuleInfo& getModuleInfo() const;

    explicit TaskGenerator(ModuleInfo aModuleInfo);
    TaskGenerator( ModuleInfo aModuleInfo, const std::function<void(Task)>& callback);

protected:
    const ModuleInfo moduleInfo;
    std::function<void(Task)> callback;
};


#endif //GREY_PARROT_RAT_SDK_TASK_GENERATOR_H
