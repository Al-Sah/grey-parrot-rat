//
// Created by alsah on 13.05.23.
//

#include <iostream>
#include "TasksMap.h"


TaskInfo TasksMap::addTaskInfo(std::uint64_t id, std::string module_id, bool toAccumulate) {

    std::lock_guard<std::mutex>lock(mutex);
    TaskInfo ti{
            .task_id = id,
            .module_id = std::move(module_id),
            .state = toAccumulate ? TaskInfo::TaskState::ACCUMULATING : TaskInfo::TaskState::REGISTERED
    };

    if (data.find(id) != data.end()) {
        std::cout << "TasksManager already contains task " << id << std::endl;
        // TODO replace with log
        return {};
    }

    data.emplace( id,ti );
    return ti;
}

void TasksMap::deleteTaskInfo(std::uint64_t id) {

    std::lock_guard<std::mutex>lock(mutex);
    data.erase(id);
}


void TasksMap::changeTaskState(std::uint64_t id, TaskInfo::TaskState newState) {
    auto ti_iter = data.find(id);
    if (ti_iter == data.end()) {
        std::cout << "Task with id " << id << " not found ??? " << std::endl;
        // TODO replace with log
        return;
    }
    ti_iter->second.state = newState;
}

TaskInfo TasksMap::getTaskInfo(std::uint64_t id) {
    auto ti_iter = data.find(id);
    if (ti_iter == data.end()) {
        std::cout << "Task with id " << id << " not found ??? " << std::endl;
        // TODO replace with log
        return {}; // TODO use optional ??
    }
    return ti_iter->second;
}

void TasksMap::addTaskInfo(const TaskInfo& ti) {
    if (data.find(ti.task_id) != data.end()) {
        std::cout << "TasksManager already contains task " << ti.task_id << std::endl;
        // TODO replace with log
        return;
    }

    data.insert( {ti.task_id,ti} );
}
