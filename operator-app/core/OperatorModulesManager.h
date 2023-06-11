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
    OperatorModulesManager(
            const std::shared_ptr<ITaskHandler> &taskHandler,
            const std::function<std::shared_ptr<rtc::DataChannel>(ModuleInfo)>& requestDataChannelCallback);

    bool handle(Task task) override;

    void registerModule(const std::shared_ptr<TaskGenerator>& taskGenerator);

    [[nodiscard]] std::vector<ModuleInfo> getModulesInfo() const override;
    [[nodiscard]] std::shared_ptr<TaskGenerator> getModule(const std::string& name);

private:

    std::map<std::string, std::shared_ptr<TaskGenerator>> modules;
    const std::function<std::shared_ptr<rtc::DataChannel>(ModuleInfo)> requestDataChannelCallback;
};


#endif //GREY_PARROT_RAT_OPERATOR_CORE_MODULES_MANAGER_H
