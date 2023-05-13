//
// Created by alsah on 12.05.23.
//

#include "TasksCreator.h"
#include "tasks-managing/TasksManager.h"

Task TasksCreator::mapSingle(const TaskInfo& taskInfo, std::vector<std::byte> payload) {
    return {
            .task_id = taskInfo.task_id,
            .module_id = taskInfo.module_id,
            .inMemory = true,
            .payload = payload,
            .isPartial = false
    };
}

Task TasksCreator::mapPartial(
        const TaskInfo& taskInfo,
        const ControlMessageHeader &header,
        std::vector<std::byte> payload) {

    return {
            .task_id = taskInfo.task_id,
            .module_id = taskInfo.module_id,
            .inMemory = true,
            .payload = payload,
            .isPartial = true,
            .part = header.getSequence(),
            .parts = header.getParts(),
            // full payload is present only if it is first part of payload
            .size = header.getFullPayloadSize()
    };
}

Task TasksCreator::mapAccumulated(const TaskInfo& taskInfo, const std::string& filepath) {
    return {
            .task_id = taskInfo.task_id,
            .module_id = taskInfo.module_id,
            .inMemory = false,
            .filepath = filepath,
            .isPartial = false
    };
}


