//
// Created by alsah on 12.06.23.
//

#include "FilesManagerOperatorApp.h"
#include "FilesManagerWidget.h"

void FilesManagerOperatorApp::handleResult(Task task) {

}

QWidget *FilesManagerOperatorApp::getUI(QWidget *parent) {
    if(moduleUi == nullptr){
        moduleUi = new FilesManagerWidget(parent);
    }
    auto *echoWidgetPtr = dynamic_cast<FilesManagerWidget*>(moduleUi);
    if(echoWidgetPtr != nullptr){
    }
    return moduleUi;
}

void FilesManagerOperatorApp::setDataChannelHandlers() {

}

FilesManagerOperatorApp::FilesManagerOperatorApp() : TaskGenerator({"files-manager", "0.0.1"}) {}
