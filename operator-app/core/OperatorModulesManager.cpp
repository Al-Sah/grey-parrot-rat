//
// Created by alsah on 13.05.23.
//

#include <iostream>
#include "OperatorModulesManager.h"

bool OperatorModulesManager::handle(Task task) {

    auto modules_iter = modules.find(task.module);
    if(modules_iter == modules.end()){
        std::cout << "OperatorModulesManager: cannot delegate task "
            << task.id << "; module ' " << task.module << "' not found" << std::endl;
        // TODO: handle error
        return false;
    }

    // TODO: run in another thread ...
    std::cout << "OperatorModulesManager: delegating response "
              << task.id << "; module '" << task.module << "'" << std::endl;
    if(pool != nullptr) {
        runningTasks.push_back(pool->submit([resultHandler = modules_iter->second, task]() {
            resultHandler->handleResult(task);
        }));
    }

    return true;
}

void OperatorModulesManager::registerModule(const std::shared_ptr<TaskGenerator>& taskGenerator) {

    taskGenerator->setCallback([this](const Task& task) {
        auto res = pool->submit([this,task](){
            // TODO: remove task from vector
            taskHandler->handle(task);
        });
        res.get();
    });
    taskGenerator->setRequestDCCallback(requestDataChannelCallback);
    modules.insert({taskGenerator->getModuleInfo().id, taskGenerator});
}

OperatorModulesManager::OperatorModulesManager(
        const std::shared_ptr<ITaskHandler> &requestsHandler,
        const std::function<std::shared_ptr<rtc::DataChannel>(ModuleInfo)> & requestDataChannelCallback
        ) : ModulesManagerBase(requestsHandler),
        requestDataChannelCallback(requestDataChannelCallback) {}


std::vector<ModuleInfo> OperatorModulesManager::getModulesInfo() const {
    std::vector<ModuleInfo> result;
    result.reserve(modules.size());

    for (const auto &item: modules){
        result.push_back(item.second->getModuleInfo());
    }
    return result;
}

std::shared_ptr<TaskGenerator> OperatorModulesManager::getModule(const std::string &name) {
    return modules[name];
}
