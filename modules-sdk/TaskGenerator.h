//
// Created by alsah on 30.05.23.
//

#ifndef GREY_PARROT_RAT_SDK_TASK_GENERATOR_H
#define GREY_PARROT_RAT_SDK_TASK_GENERATOR_H


#include <functional>
#include "Task.h"
#include "ModuleInfo.h"
#include "rtc/datachannel.hpp"

class QWidget;

class TaskGenerator {

public:
    virtual void handleResult(Task task) = 0;

    [[nodiscard]] const ModuleInfo& getModuleInfo() const;

    explicit TaskGenerator(ModuleInfo aModuleInfo);
    TaskGenerator( ModuleInfo aModuleInfo, const std::function<void(Task)>& callback);

    void setCallback(const std::function<void(Task)> &aCallback);

    virtual QWidget* getUI(QWidget* parent) = 0;


    void closeDataChanel();
    void setRequestDCCallback(
            const std::function<std::shared_ptr<rtc::DataChannel>(ModuleInfo)> &requestDataChannelCallback);

protected:

    virtual void setDataChannelHandlers() = 0;

    const ModuleInfo moduleInfo;
    std::function<void(Task)> callback;
    std::function<std::shared_ptr<rtc::DataChannel>(ModuleInfo)> requestDataChannelCallback;

    std::shared_ptr<rtc::DataChannel> dataChannel;
};


#endif //GREY_PARROT_RAT_SDK_TASK_GENERATOR_H
