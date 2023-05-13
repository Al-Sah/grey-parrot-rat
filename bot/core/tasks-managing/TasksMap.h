//
// Created by alsah on 13.05.23.
//

#ifndef GREY_PARROT_CLIENTS_TASKSMAP_H
#define GREY_PARROT_CLIENTS_TASKSMAP_H


#include <cstdint>
#include <map>
#include <mutex>

#include "TaskInfo.h"

/**
 * @class TasksMap
 * This class is a thread-safe wrapper for std::map that contains information about tasks
 */
class TasksMap {

private:
    // underlying type std::map where uint64_t - task id
    std::map<std::uint64_t, TaskInfo> data;
    std::mutex mutex;

public:
    TasksMap() = default;

    // Basic CRUD operation

    void addTaskInfo(const TaskInfo& ti);
    TaskInfo addTaskInfo(std::uint64_t id, std::string module_id, bool toAccumulate);
    void deleteTaskInfo(std::uint64_t id);
    void changeTaskState(std::uint64_t id, TaskInfo::TaskState newState);
    TaskInfo getTaskInfo(std::uint64_t id);
};


#endif //GREY_PARROT_CLIENTS_TASKSMAP_H
