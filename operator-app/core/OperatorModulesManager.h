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
#include "modules-managing/ModulesManagerBase.h"

class OperatorModulesManager : public ModulesManagerBase{

public:
    explicit OperatorModulesManager(const std::shared_ptr<ITaskHandler> &taskHandler);
    bool handle(Task task) override;
    void registerModule(const std::shared_ptr<TaskGenerator>& taskGenerator);
    [[nodiscard]] std::vector<ModuleInfo> getModulesInfo() const override;

private:

    std::map<std::string, std::shared_ptr<TaskGenerator>> modules;

};


#endif //GREY_PARROT_RAT_OPERATOR_CORE_MODULES_MANAGER_H
