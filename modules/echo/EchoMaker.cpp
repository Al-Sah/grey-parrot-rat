//
// Created by alsah on 10.06.23.
//

#include <iostream>
#include "EchoMaker.h"
#include "EchoWidget.h"
#include "general-data.pb.h"

void EchoMaker::handleResult(Task task) {
    std::cout << "EchoMaker: received response" << std::endl;

    msgs::FileData fileData{};
    bool res = fileData.ParseFromString(std::get<std::string>(task.payload));
    if(res){
        emit bridge->onFile(fileData.filename());
    } else{
        emit bridge->onText(std::get<std::string>(task.payload));
    }
}


QWidget *EchoMaker::getUI(QWidget *parent) {

    if(moduleUi == nullptr){
        moduleUi = new EchoWidget(parent, this);
        bridge = new EchoModuleBridge(moduleUi);
    }
    auto *echoWidgetPtr = dynamic_cast<EchoWidget*>(moduleUi);
    if(echoWidgetPtr != nullptr){
        echoWidgetPtr->clearUI();
    }
    return moduleUi;
}

EchoMaker::EchoMaker() : TaskGenerator({"echo-test", "0.0.0"}) {}


void EchoMaker::sendText(const std::string& data) {

    if(dataChannel != nullptr && dataChannel->isOpen()){
        dataChannel->send(data);
        return;
    }

    // TODO: use random generator ?
    auto res = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count();

    Task request{
        .id = static_cast<uint64_t>(res),
        .module = moduleInfo.id,
        .payload = data,
        .isFilepath = false,
        .asPeer = true,
    };
    callback(request);
}

void EchoMaker::sendFile(const std::string &filepath) {
    // TODO: use random generator ?
    auto res = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();

    Task request{
            .id = static_cast<uint64_t>(res),
            .module = moduleInfo.id,
            .payload = filepath,
            .isFilepath = true,
            .asPeer = true,
    };
    callback(request);
}


EchoMaker::~EchoMaker() {
    delete bridge;
    delete moduleUi;
}


void EchoMaker::requestDataChannel() {

    dataChannel = requestDataChannelCallback(moduleInfo);
    setDataChannelHandlers();

}

void EchoMaker::setDataChannelHandlers() {

    dataChannel->onOpen([this]() {
        std::cout << "ECHO MODULE: Opened new data channel" << std::endl;
        emit bridge->onDataChannel(true);
    });

    dataChannel->onError([](const std::string& error){
        std::cout << "ECHO MODULE: data channel error: " << error << std::endl;

    });

    dataChannel->onClosed([this]() {
        std::cout << "ECHO MODULE: DataChannel closed" << std::endl;
        emit bridge->onDataChannel(false);
    });

    dataChannel->onMessage([this](auto data) {

        if (!std::holds_alternative<rtc::string>(data)) {
            std::cout << "ECHO MODULE: Data channel - received message is not string" << std::endl;
            return;
        }
        emit bridge->onText(std::get<std::string>(data));
    });

}

