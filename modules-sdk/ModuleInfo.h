//
// Created by alsah on 14.05.23.
//

#ifndef GREY_PARROT_SDK_MODULE_INFO_H
#define GREY_PARROT_SDK_MODULE_INFO_H


#include <string>
#include <utility>

struct ModuleInfo{
    const std::string id;
    const std::string version;

    ModuleInfo(std::string aId, std::string aVersion) :
        id(std::move(aId)),
        version(std::move(aVersion)){}
};

#endif //GREY_PARROT_SDK_MODULE_INFO_H
