//
// Created by alsah on 13.05.23.
//

#ifndef GREY_PARROT_BOT_TASK_INFO_MODEL_H
#define GREY_PARROT_BOT_TASK_INFO_MODEL_H

#include <string>

/**
 * @struct TaskInfo
 *
 * Contains basic information about task and it state.
 */
struct TaskInfo{

    enum class TaskState{

        REGISTERED,   // waiting to the free thread to start executing
        ACCUMULATING, // is used if module can not handle partial messages
        EXECUTING,    // task was passed to the execution
        CLOSING       // module requested to close task
    };

    const std::uint64_t task_id;
    const std::string module_id;

    TaskState state;
};

#endif //GREY_PARROT_BOT_TASK_INFO_MODEL_H