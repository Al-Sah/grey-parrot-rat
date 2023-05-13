//
// Created by alsah on 13.05.23.
//

#include "ModulesManager.h"

void ModulesManager::delegate(Task task) {

    auto modules_iter = modules.find(task.module_id);
    if(modules_iter == modules.end()){
        // TODO: handle error
        return;
    }
    // TODO: run in another thread ...
    modules_iter->second->execute(task);
}

void ModulesManager::registerModule(const std::shared_ptr<TaskExecutor>& taskExecutor) {
    modules.emplace(taskExecutor->getModuleId(), taskExecutor);
}
