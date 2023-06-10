//
// Created by alsah on 13.05.23.
//

#include <iostream>
#include "AgentModulesManager.h"

bool AgentModulesManager::handle(Task task) {

    auto modules_iter = modules.find(task.module);
    if(modules_iter == modules.end()){
        std::cout << "TaskExecutorsManager: cannot delegate task "
            << task.id << "; module ' " << task.module << "' not found" << std::endl;
        // TODO: handle error
        return false;
    }
    // TODO: run in another thread ...
    std::cout << "TaskExecutorsManager: delegating task "
              << task.id << "; module '" << task.module << "'" << std::endl;
    modules_iter->second->execute(task);
    return true;
}


void AgentModulesManager::registerModule(const std::shared_ptr<TaskExecutor>& taskExecutor) {

    taskExecutor->setCallback([this](const Task& task) {
        // TODO: run in separate thread
        taskHandler->handle(task);
    });

    modules.emplace(taskExecutor->getModuleInfo().id, taskExecutor);
}

std::vector<ModuleInfo> AgentModulesManager::getModulesInfo() const {
    auto result = std::vector<ModuleInfo>();
    result.reserve(modules.size());

    for (const auto &item: modules){
        result.emplace_back(item.second->getModuleInfo());
    }

    return result;
}

AgentModulesManager::AgentModulesManager(const std::shared_ptr<ITaskHandler> &requestsHandler)
    : ModulesManagerBase(requestsHandler) {}
