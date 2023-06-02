//
// Created by alsah on 13.05.23.
//

#ifndef GREY_PARROT_RAT_SDK_TASK_INFO_H
#define GREY_PARROT_RAT_SDK_TASK_INFO_H

#include <string>

/**
 * @struct TaskInfo
 *
 * Contains basic information about task and it state
 */
struct TaskInfo{

    enum class TaskState{

        CREATED,    // TaskGenerator created task
        SENT,       // Task was sent to the agent (agents)
        REGISTERED, // Task was registered; not passed to execution yet
        EXECUTING,  // Task was passed to the execution
        CLOSED      // TaskExecutor sent closing result message
    };

    const std::uint64_t id;
    const std::string module;

    TaskState state;
};

#endif //GREY_PARROT_RAT_SDK_TASK_INFO_H