//
// Created by alsah on 12.06.23.
//

#include "ProcessManagerOperator.h"
#include "RunningTasksWidget.h"

void ProcessManagerOperator::handleResult(Task task) {

}

QWidget *ProcessManagerOperator::getUI(QWidget *parent) {
    if(moduleUi == nullptr){
        moduleUi = new RunningTasksWidget(parent);
    }
    auto *echoWidgetPtr = dynamic_cast<RunningTasksWidget*>(moduleUi);
    if(echoWidgetPtr != nullptr){
    }
    return moduleUi;
}

void ProcessManagerOperator::setDataChannelHandlers() {

}

ProcessManagerOperator::ProcessManagerOperator() : TaskGenerator({"process-manager","0.0.1"}) {}
