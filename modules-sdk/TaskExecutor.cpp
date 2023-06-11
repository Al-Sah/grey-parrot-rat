//
// Created by alsah on 10.05.23.
//

#include "TaskExecutor.h"

TaskExecutor::TaskExecutor(ModuleInfo aModuleInfo):
        moduleInfo(std::move(aModuleInfo)){}

TaskExecutor::TaskExecutor(ModuleInfo aModuleInfo, const std::function<void(Task)> &callback):
        moduleInfo(std::move(aModuleInfo)),
        callback(callback){}

void TaskExecutor::setCallback(const std::function<void(Task)> &aCallback) {
    TaskExecutor::callback = aCallback;
}

ModuleInfo TaskExecutor::getModuleInfo() const {
    return moduleInfo;
}

void TaskExecutor::setDataChannel(const std::shared_ptr<rtc::DataChannel> &dataChannel) {
    TaskExecutor::dataChannel = dataChannel;
    setDataChannelHandlers();
}

void TaskExecutor::closeDataChanel() {

}
