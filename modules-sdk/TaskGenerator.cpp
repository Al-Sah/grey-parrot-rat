//
// Created by alsah on 30.05.23.
//

#include <memory>
#include "TaskGenerator.h"
#include "rtc/datachannel.hpp"

const ModuleInfo& TaskGenerator::getModuleInfo() const {
    return moduleInfo;
}

TaskGenerator::TaskGenerator(ModuleInfo aModuleInfo):
        moduleInfo(std::move(aModuleInfo)){}

TaskGenerator::TaskGenerator(ModuleInfo aModuleInfo, const std::function<void(Task)> &callback):
        moduleInfo(std::move(aModuleInfo)),
        callback(callback){}

void TaskGenerator::setCallback(const std::function<void(Task)> &aCallback) {
    this->callback = aCallback;
}


void TaskGenerator::setRequestDCCallback(
        const std::function<std::shared_ptr<rtc::DataChannel>(ModuleInfo)> &requestDataChannelCallback) {
    TaskGenerator::requestDataChannelCallback = requestDataChannelCallback;
}

void TaskGenerator::closeDataChanel() {
    this->dataChannel->close();
    this->dataChannel = nullptr;
}

