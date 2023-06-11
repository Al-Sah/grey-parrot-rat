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

    if(response.isFilepath){
        std::filesystem::path path{std::get<std::string>(response.payload)};

        if(!std::filesystem::exists(path)){
            return false;
        }
        std::size_t filesize = std::filesystem::file_size(path);

        if(messagesSender->getMaxSize(!response.asPeer) < 50){
            return false;
        }

        if(messagesSender->getMaxSize(!response.asPeer) - 50 > filesize){
            controlPacket.set_payload(loadFile(path).SerializeAsString());
        } else{
            std::cout << "Cannot handle file; size: " << filesize << std::endl;
            return false;
        }

    } else{
        controlPacket.set_payload(std::get<std::string>(response.payload));
    }

    // TODO: add other types
    header->set_type(msgs::ControlHeader_MessageType_SINGLE);
    header->set_module(response.module);
    header->set_requestid(response.id);
    header->set_isclosing(response.isClosing);
    header->set_peer(response.asPeer);

    messagesSender->send(controlPacket);

    // TODO: log it
    std::cout << "Sending ControlPacket | module: " << response.module << ", task_id: " << response.id << std::endl;

    return true;
}


