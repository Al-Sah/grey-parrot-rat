//
// Created by alsah on 10.05.23.
//

#ifndef GREY_PARROT_SDK_MODULE_INTERFACE_H
#define GREY_PARROT_SDK_MODULE_INTERFACE_H


#include <string>
#include <functional>
#include "TaskResult.h"

class Module {

protected:
    const std::string moduleId;
    const std::string version;
    const std::function<void(TaskResult)> callback;

public:

    [[nodiscard]] std::string getModuleId() const{
        return this->moduleId;
    }
    [[nodiscard]] std::string getVersion() const {
        return this->version;
    }

};


#endif //GREY_PARROT_SDK_MODULE_INTERFACE_H
