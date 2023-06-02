//
// Created by alsah on 01.06.23.
//

#include "AgentTasksManager.h"
#include "tasks-managing/TasksMapper.h"


void AgentTasksManager::handleInbox(msgs::ControlPacket packet) {
    switch (packet.header().type()) {
        case msgs::ControlHeader_MessageType_SINGLE:
        {
            tasks.addTaskInfo(packet.header().requestid(), packet.header().module());
            taskHandler->handle(TasksMapper::mapSingle(packet));
        }
            break;
        case msgs::ControlHeader_MessageType_FIRST:
        case msgs::ControlHeader_MessageType_NEXT:
        case msgs::ControlHeader_MessageType_LAST:
        {
            TaskInfo ti = tasks.getTaskInfo(packet.header().requestid());
            taskHandler->handle(TasksMapper::mapPartial(ti, packet));
        }
            break;
        default:
            break;
    }
}

bool AgentTasksManager::handle(Task response) {

    if(response.isClosing){
        tasks.deleteTaskInfo(response.id);
    }

    msgs::ControlPacket controlPacket{};
    msgs::ControlHeader *header = controlPacket.mutable_header();

    // TODO: add other types
    header->set_type(msgs::ControlHeader_MessageType_SINGLE);
    header->set_module(response.module);
    header->set_requestid(response.id);
    header->set_isclosing(response.isClosing);

    controlPacket.set_payload(std::get<std::string>(response.payload));


    messagesSender->send(controlPacket);

    // TODO: log it
    std::cout << "Sending ControlPacket:\n"
              << " | module: " << response.module << std::endl
            << " | task_id: " << response.id << std::endl
            << " | closing: " << response.isClosing << std::endl
            << " | in ram: " << response.isFilepath << std::endl;

    return true;
}


