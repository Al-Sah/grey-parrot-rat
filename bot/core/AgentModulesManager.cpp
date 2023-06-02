//
// Created by alsah on 13.05.23.
//

#include <iostream>
#include <list>
#include "AgentModulesManager.h"

bool AgentModulesManager::handle(Task task) {

    auto modules_iter = modules.find(task.module);
    if(modules_iter == modules.end()){
        std::cout << "TaskExecutorsManager: cannot delegate task "
            << task.id << "; module ' " << task.module << "' not found";
        // TODO: handle error
        return false;
    }
    // TODO: run in another thread ...
    std::cout << "TaskExecutorsManager: delegating task "
              << task.id << "; module ' " << task.module << "'" << std::endl;
    modules_iter->second->execute(task);
    return true;
}


void AgentModulesManager::registerModule(const std::shared_ptr<TaskExecutor>& taskExecutor) {
    modules.emplace(taskExecutor->getModuleInfo().id, taskExecutor);
}

std::list<ModuleInfo> AgentModulesManager::getExecutorsInfo() const {
    auto result = std::list<ModuleInfo>();

    for (const auto &item: modules){
        result.emplace_back(item.second->getModuleInfo());
    }

    return result;
}

