//
// Created by alsah on 12.05.23.
//

#include "TasksMapper.h"

Task TasksMapper::mapSingle(const msgs::ControlPacket& packet) {
    return {
            .id = packet.header().requestid(),
            .module = packet.header().module(),
            .payload = packet.payload(),
            .isFilepath = false,
            .isClosing = packet.header().isclosing(),

            .isPartial = false,
            .part = std::nullopt,
            .parts = std::nullopt,
            .size = std::nullopt
    };
}

Task TasksMapper::mapPartial(const TaskInfo& taskInfo, const msgs::ControlPacket& packet) {
    return {
            .id = taskInfo.id,
            .module = taskInfo.module,
            .payload = packet.payload(),
            .isFilepath = false,
            .isClosing = packet.header().isclosing(),

            .isPartial = true,
            .part = packet.header().partition(),
            .parts = packet.header().parts(),
            .size = packet.header().payloadsize()
    };
}

msgs::ControlPacket TasksMapper::mapSingle(const Task &task) {

    msgs::ControlPacket controlPacket{};
    msgs::ControlHeader *header = controlPacket.mutable_header();

    header->set_type(msgs::ControlHeader_MessageType_SINGLE);
    header->set_requestid(task.id);

    header->set_module(task.module);
    header->set_isclosing(task.isClosing);

    setPayload(task, controlPacket);

    return controlPacket;
}


msgs::ControlPacket TasksMapper::mapPartial(const Task &task) {

    msgs::ControlPacket controlPacket{};
    //msgs::ControlHeader *header = controlPacket.mutable_header();
    // TODO: implement

    return controlPacket;
}


void TasksMapper::setPayload(const Task &task, msgs::ControlPacket &packet) {

    bool const isBinary = std::holds_alternative<std::vector<std::byte>>(task.payload);

    if(isBinary){
        // convert std::vector<std::byte> to std::string
        auto binary = std::get<std::vector<std::byte>>(task.payload);
        std::string strPayload = std::string(
                reinterpret_cast<const char*>(binary.data()),
                binary.size()
        );

        packet.set_payload(strPayload);
    } else {
        packet.set_payload(std::get<std::string>(task.payload));
    }
}



