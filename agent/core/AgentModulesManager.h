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

class AgentModulesManager : public ITaskHandler{

private:

    std::map<std::string, std::shared_ptr<TaskExecutor>> modules;

public:
    bool handle(Task data) override;

    void registerModule(const std::shared_ptr<TaskExecutor>& taskExecutor);

    [[nodiscard]] std::list<ModuleInfo> getExecutorsInfo() const;
};


#endif //GREY_PARROT_AGENT_MODULES_MANAGER_H
