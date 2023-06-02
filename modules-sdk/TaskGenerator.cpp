//
// Created by alsah on 30.05.23.
//

#include "TaskGenerator.h"

const ModuleInfo& TaskGenerator::getModuleInfo() const {
    return moduleInfo;
}

TaskGenerator::TaskGenerator(ModuleInfo aModuleInfo):
        moduleInfo(std::move(aModuleInfo)){}

TaskGenerator::TaskGenerator(ModuleInfo aModuleInfo, const std::function<void(Task)> &callback):
        moduleInfo(std::move(aModuleInfo)),
        callback(callback){}