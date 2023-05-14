//
// Created by alsah on 10.05.23.
//

#include "TaskExecutor.h"

TaskExecutor::TaskExecutor(ModuleInfo aModuleInfo):
        moduleInfo(std::move(aModuleInfo)){}

TaskExecutor::TaskExecutor(ModuleInfo aModuleInfo, const std::function<void(TaskResult)> &callback):
        moduleInfo(std::move(aModuleInfo)),
        callback(callback){}

void TaskExecutor::setCallback(const std::function<void(TaskResult)> &aCallback) {
    TaskExecutor::callback = aCallback;
}

std::string TaskExecutor::getModuleVersion() const {
    return this->moduleInfo.version;
}

std::string TaskExecutor::getModuleId() const {
    return this->moduleInfo.id;
}
