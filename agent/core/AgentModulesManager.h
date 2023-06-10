//
// Created by alsah on 13.05.23.
//

#ifndef GREY_PARROT_AGENT_MODULES_MANAGER_H
#define GREY_PARROT_AGENT_MODULES_MANAGER_H

#include <map>
#include <memory>
#include <list>


#include "TaskExecutor.h"
#include "ITaskHandler.h"
#include "modules-managing/ModulesManagerBase.h"

class AgentModulesManager : public ModulesManagerBase{

public:
    explicit AgentModulesManager(const std::shared_ptr<ITaskHandler> &requestsHandler);

    bool handle(Task data) override;
    void registerModule(const std::shared_ptr<TaskExecutor>& taskExecutor);
    [[nodiscard]] std::vector<ModuleInfo> getModulesInfo() const override;

private:
    std::map<std::string, std::shared_ptr<TaskExecutor>> modules;
};


#endif //GREY_PARROT_AGENT_MODULES_MANAGER_H
