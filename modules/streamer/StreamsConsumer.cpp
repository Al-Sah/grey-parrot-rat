//
// Created by alsah on 11.06.23.
//

#include "StreamsConsumer.h"
#include "StreamerWidget.h"

void StreamsConsumer::handleResult(Task task) {

}

QWidget *StreamsConsumer::getUI(QWidget *parent) {
    if(moduleUi == nullptr){
        moduleUi = new StreamerWidget(parent);
    }
    auto *echoWidgetPtr = dynamic_cast<StreamerWidget*>(moduleUi);
    if(echoWidgetPtr != nullptr){
    }
    return moduleUi;
}

void StreamsConsumer::setDataChannelHandlers() {

}

StreamsConsumer::StreamsConsumer() : TaskGenerator({"streamer","0.0.1"}) {}
