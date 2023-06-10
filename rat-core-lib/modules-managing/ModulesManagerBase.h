//
// Created by alsah on 10.06.23.
//

#ifndef GREY_PARROT_RAT_CORE_MODULES_MANAGER_BASE_H
#define GREY_PARROT_RAT_CORE_MODULES_MANAGER_BASE_H

#include <memory>
#include "TaskGenerator.h"
#include "ITaskHandler.h"


class ModulesManagerBase : public ITaskHandler{

public:
    explicit ModulesManagerBase(const std::shared_ptr<ITaskHandler> &requestsHandler);
    virtual ~ModulesManagerBase() = default;

    [[nodiscard]] virtual std::vector<ModuleInfo> getModulesInfo() const = 0;

protected:

    // handler to pass "Task" from module to the core
    const std::shared_ptr<ITaskHandler> taskHandler;
};


#endif //GREY_PARROT_RAT_CORE_MODULES_MANAGER_BASE_H
