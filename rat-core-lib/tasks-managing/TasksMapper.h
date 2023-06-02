//
// Created by alsah on 12.05.23.
//

#ifndef GREY_PARROT_RAT_CORE_TASKS_MAPPER_H
#define GREY_PARROT_RAT_CORE_TASKS_MAPPER_H


#include "control.pb.h"

#include "Task.h"
#include "TaskInfo.h"


class TasksMapper {

public:

    /**
     * Convert 'single' control message into task
     * @param taskInfo - basic information about task
     * @param payload - data in memory
     * @return task that can be passed to execution
     */
    static Task mapSingle(const msgs::ControlPacket& packet);

    /**
     * Convert 'first' / 'continuation' / 'last' control message into task
     * @param taskInfo - basic information about task
     * @param header - must contains valid information of this part
     * @param payload - data in memory
     * @return task that can be passed to execution
     */
    static Task mapPartial(const TaskInfo& taskInfo, const msgs::ControlPacket& packet);


    static msgs::ControlPacket mapSingle(const Task& task);

    static msgs::ControlPacket mapPartial(const Task& task);


private:
    static void setPayload(const Task& task, msgs::ControlPacket& packet);
};


#endif //GREY_PARROT_RAT_CORE_TASKS_MAPPER_H
