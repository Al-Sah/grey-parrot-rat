//
// Created by alsah on 01.06.23.
//

#include "OperatorTasksManager.h"
#include "tasks-managing/TasksMapper.h"

#include <filesystem>
#include <fstream>

#include "general-data.pb.h"

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

    tasks.addTaskInfo(data.id, data.module);

    if(onTasksCountChange != nullptr){
        onTasksCountChange(tasks.getSize());
    }

    msgs::ControlPacket controlPacket{};
    msgs::ControlHeader *header = controlPacket.mutable_header();


    if(data.isFilepath){
        std::filesystem::path path{std::get<std::string>(data.payload)};

        if(!std::filesystem::exists(path)){
            return false;
        }
        std::size_t filesize = std::filesystem::file_size(path);

        if(messagesSender->getMaxSize(!data.asPeer) < 50){
            return false;
        }

        if(messagesSender->getMaxSize(!data.asPeer) - 50 > filesize){
            controlPacket.set_payload(loadFile(path).SerializeAsString());
        } else{
            std::cout << "Cannot handle file; size: " << filesize << std::endl;
            return false;
        }
    } else{
        controlPacket.set_payload(std::get<std::string>(data.payload));
    }


    // TODO: add other types
    header->set_type(msgs::ControlHeader_MessageType_SINGLE);
    header->set_module(data.module);
    header->set_requestid(data.id);
    header->set_isclosing(data.isClosing);
    header->set_peer(data.asPeer);

    messagesSender->send(controlPacket);

    // TODO
    // if connection is open - send; else put to queue
    return true;
}
