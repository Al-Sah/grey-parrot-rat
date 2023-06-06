//
// Created by alsah on 13.05.23.
//

#ifndef GREY_PARROT_RAT_OPERATOR_CORE_MODULES_MANAGER_H
#define GREY_PARROT_RAT_OPERATOR_CORE_MODULES_MANAGER_H

#include <map>
#include <memory>
#include <list>


#include "TaskGenerator.h"
#include "ITaskHandler.h"

class OperatorModulesManager : public ITaskHandler{

public:
    explicit OperatorModulesManager(const std::shared_ptr<ITaskHandler> &requestsHandler);

    bool handle(Task task) override;

    void registerModule(const std::shared_ptr<TaskGenerator>& taskGenerator);

private:

    std::map<std::string, std::shared_ptr<TaskGenerator>> modules;

    // handler to pass "Task" from module to the core
    const std::shared_ptr<ITaskHandler> requestsHandler;
};


#endif //GREY_PARROT_RAT_OPERATOR_CORE_MODULES_MANAGER_H
