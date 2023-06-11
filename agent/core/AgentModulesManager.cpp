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

    std::cout << "TaskExecutorsManager: delegating task " << task.id << "; module '" << task.module << "'" << std::endl;
    if(pool != nullptr) {
        runningTasks.push_back(pool->submit([taskExecutor = modules_iter->second, task]() {
            taskExecutor->execute(task);
        }));
    }

    return true;
}


void AgentModulesManager::registerModule(const std::shared_ptr<TaskExecutor>& taskExecutor) {

    taskExecutor->setCallback([this](const Task& task) {
        auto res = pool->submit([this,task](){
            // TODO: remove task from vector
            taskHandler->handle(task);
        });
        res.get();
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

AgentModulesManager::~AgentModulesManager() {
    using namespace std::chrono_literals;
    for (const auto &item: runningTasks){
        item.wait_for(std::chrono::microseconds(1s));
    }
}

void AgentModulesManager::passDataChannel(const std::shared_ptr<rtc::DataChannel>& dc) {
    auto taskExecutor = modules[dc->label()];
    taskExecutor->setDataChannel(dc);
}
