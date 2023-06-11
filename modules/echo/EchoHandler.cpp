//
// Created by alsah on 10.06.23.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include "EchoHandler.h"

#include "general-data.pb.h"

void EchoHandler::execute(Task task) {

    std::string payload;
    bool isFile{false};

    if (!std::holds_alternative<std::string>(task.payload)) {
        std::cout << "ECHO MODULE: received invalid binary data" << std::endl;
    }

    msgs::FileData fileData{};
    bool res = fileData.ParseFromString(std::get<std::string>(task.payload));
    if(res){
        payload = fileData.SerializeAsString();
        std::filesystem::path path = std::filesystem::temp_directory_path();
        path = path / fileData.filename();
        std::ofstream echoFile(path.string());
        echoFile << fileData.data();
        echoFile.close();
        payload = path.string();
        isFile = true;
    } else{
        payload = std::get<std::string>(task.payload) + " (plain reply) ";
    }

    Task result{
            .id = task.id,
            .module = moduleInfo.id,
            .payload = payload,
            .isFilepath = isFile,
            .isClosing = true,
            .isPartial = task.isPartial,
            .asPeer = task.asPeer
    };
    callback(result);
}

EchoHandler::EchoHandler() : TaskExecutor({"echo-test", "0.0.0"}) {}

void EchoHandler::setDataChannelHandlers() {

    dataChannel->onOpen([]() {
        std::cout << "ECHO MODULE: Opened new data channel" << std::endl;
    });

    dataChannel->onError([](const std::string& error){
        std::cout << "ECHO MODULE: data channel error: " << error << std::endl;
    });

    dataChannel->onClosed([]() {
        std::cout << "ECHO MODULE: DataChannel closed" << std::endl;
    });

    dataChannel->onMessage([this](auto data) {

        if (std::holds_alternative<std::string>(data)) {
            std::string payload = std::get<std::string>(data);
            payload.append(" (direct channel reply) ");
            dataChannel->send(payload);
        } else if (std::holds_alternative<rtc::binary>(data)) {
            dataChannel->send(data);
        }
    });
}
