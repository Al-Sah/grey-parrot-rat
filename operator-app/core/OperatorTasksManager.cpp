//
// Created by alsah on 01.06.23.
//

#include "OperatorTasksManager.h"
#include "tasks-managing/TasksMapper.h"

void OperatorTasksManager::handleInbox(msgs::ControlPacket packet) {

    switch (packet.header().type()) {
        case msgs::ControlHeader_MessageType_SINGLE:
        {
            //tasks.addTaskInfo(packet.header().requestid(), packet.header().module());
            taskHandler->handle(TasksMapper::mapSingle(packet));
        }
            break;
        case msgs::ControlHeader_MessageType_FIRST:
        case msgs::ControlHeader_MessageType_NEXT:
        case msgs::ControlHeader_MessageType_LAST:
        {
            //TaskInfo ti = tasks.getTaskInfo(packet.header().requestid());
            //taskHandler->handle(TasksMapper::mapPartial(ti, packet));
        }
            break;
        default:
            break;
    }
}

bool OperatorTasksManager::handle(Task data) {

    // TODO
    // if connection is open - send; else put to queue
    return true;
}
