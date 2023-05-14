//
// Created by alsah on 12.05.23.
//

#ifndef GREY_PARROT_CLIENTS_TASKSCREATOR_H
#define GREY_PARROT_CLIENTS_TASKSCREATOR_H


#include <Task.h>
#include "networking/ControlMessageHeader.h"
#include "tasks-managing/TasksManager.h"


class TasksCreator {

public:

    /**
     * Convert 'single' control message into task
     * @param taskInfo - basic information about task
     * @param payload - data in memory
     * @return task that can be passed to execution
     */
    static Task mapSingle(const TaskInfo& taskInfo, std::vector<std::byte> payload);

    /**
     * Convert 'last' control message into task (if task had to be accumulated)
     * @param taskInfo - basic information about task
     * @param filepath - payload had to be accumulated in file
     * @return task that can be passed to execution
     */
    static Task mapAccumulated(const TaskInfo& taskInfo, const std::string& filepath);

    /**
     * Convert 'first' / 'continuation' / 'last' control message into task
     * @param taskInfo - basic information about task
     * @param header - must contains valid information of this part
     * @param payload - data in memory
     * @return task that can be passed to execution
     */ // TODO: replace header with some structure that contains 'partial' info
    static Task mapPartial(
            const TaskInfo& taskInfo,
            const ControlMessageHeader& header,
            std::vector<std::byte> payload);
};


#endif //GREY_PARROT_CLIENTS_TASKSCREATOR_H
