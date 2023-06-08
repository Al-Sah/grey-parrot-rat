//
// Created by alsah on 13.05.23.
//

#include <iostream>
#include "OperatorModulesManager.h"

bool OperatorModulesManager::handle(Task task) {

    auto modules_iter = modules.find(task.module);
    if(modules_iter == modules.end()){
        std::cout << "OperatorModulesManager: cannot delegate task "
            << task.id << "; module ' " << task.module << "' not found";
        // TODO: handle error
        return false;
    }
    // TODO: run in another thread ...
    std::cout << "OperatorModulesManager: delegating response "
              << task.id << "; module ' " << task.module << "'";
    modules_iter->second->handleResult(task);

    return true;
}

void OperatorModulesManager::registerModule(const std::shared_ptr<TaskGenerator>& taskGenerator) {
    taskGenerator->setCallback([this](const Task& task) {requestsHandler->handle(task);});
    modules.insert({taskGenerator->getModuleInfo().id, taskGenerator});
}

OperatorModulesManager::OperatorModulesManager(
        const std::shared_ptr<ITaskHandler> &requestsHandler
        ) : requestsHandler(requestsHandler) {}

std::vector<ModuleInfo> OperatorModulesManager::getModulesInfo() const {
    std::vector<ModuleInfo> result;
    result.reserve(modules.size());

    for (const auto &item: modules){
        result.push_back(item.second->getModuleInfo());
    }
    return result;
}
