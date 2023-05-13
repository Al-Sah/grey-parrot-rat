//
// Created by alsah on 08.05.23.
//

#include <iostream>
#include "TasksManager.h"

#include "../TasksCreator.h"

void TasksManager::addTask(ControlMessageHeader header, std::vector<std::byte> payload) {

    std::cout << "Received new task: " << header.getRequestId() << std::endl;

    switch (header.getMessageType()) {

        case ControlMessageHeader::MessageType::SINGLE: {
            Task task = handleSingleMessage(header, payload);
            // TODO pass this to the modules manager
        }
            break;
        case ControlMessageHeader::MessageType::FIRST: {

            // TODO pass this to the modules manager
        }
            break;
        case ControlMessageHeader::MessageType::NEXT:
            // header.getAccumulate().value() ??
            // pass to the module manager or accumulate
            break;
        case ControlMessageHeader::MessageType::LAST:
            // pass to the module manager
            break;
    }

}


Task TasksManager::handleSingleMessage(const ControlMessageHeader &header, std::vector<std::byte> payload) {

    TaskInfo ti = tasks.addTaskInfo(header.getRequestId(), header.getModuleId().value(), false);

    return TasksCreator::mapSingle(ti, payload);
}


std::optional<Task> TasksManager::handleFirstMessage(
        const ControlMessageHeader &header, std::vector<std::byte> payload) {

    TaskInfo ti = tasks.addTaskInfo(header.getRequestId(), header.getModuleId().value(),  header.getAccumulate().value());

    if(header.getAccumulate().value()){
        // TODO: save to tmp storage
        return {}; // empty optional
    }

    return TasksCreator::mapPartial(ti, header, payload);

}

Task TasksManager::handleLastMessage(const ControlMessageHeader &header, std::vector<std::byte> payload) {

    TaskInfo ti = tasks.getTaskInfo(header.getRequestId());

    if(header.getAccumulate().value()){
        // TODO: get path to saved task
        return TasksCreator::mapAccumulated(ti, "");
    }

    return TasksCreator::mapPartial(ti, header, payload);

}

std::optional<Task> TasksManager::handleContinuationMessage(
        const ControlMessageHeader &header,
        std::vector<std::byte> payload) {


    TaskInfo ti = tasks.getTaskInfo(header.getRequestId());

    if(header.getAccumulate().value()){
        // TODO: save to tmp storage
        return {}; // empty optional
    }

    return TasksCreator::mapPartial(ti, header, payload);
}


