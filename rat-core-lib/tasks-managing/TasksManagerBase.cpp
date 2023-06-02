//
// Created by alsah on 08.05.23.
//

#include "TasksManagerBase.h"


void TasksManagerBase::setMessagesSender(const std::shared_ptr<IControlPacketSender> &aMessagesSender) {
    this->messagesSender = aMessagesSender;
}

void TasksManagerBase::setTaskHandler(const std::shared_ptr<ITaskHandler> &aTaskHandler) {
    this->taskHandler = aTaskHandler;
}