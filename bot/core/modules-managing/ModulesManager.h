//
// Created by alsah on 13.05.23.
//

#ifndef GREY_PARROT_BOT_MODULES_MANAGER_H
#define GREY_PARROT_BOT_MODULES_MANAGER_H

#include <map>
#include <memory>

#include "ITaskDelegator.h"
#include "../../../modules-sdk/TaskExecutor.h"

class ModulesManager : public ITaskDelegator{

private:

    std::map<std::string, std::shared_ptr<TaskExecutor>> modules;

public:
    void delegate(Task task) override;

    void registerModule(const std::shared_ptr<TaskExecutor>& taskExecutor);
};


#endif //GREY_PARROT_BOT_MODULES_MANAGER_H
