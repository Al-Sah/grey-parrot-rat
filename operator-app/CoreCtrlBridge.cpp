//
// Created by alsah on 06.06.23.
//

#include "CoreCtrlBridge.h"

#include "gui/MainWindow.h"

CoreCtrlBridge::CoreCtrlBridge(QWidget *mw) {

    qRegisterMetaType<msgs::ActiveAgents>("ActiveAgents");
    qRegisterMetaType<std::string>("stdString");

    connect(this, &CoreCtrlBridge::onActiveAgents, dynamic_cast<MainWindow*>(mw), &MainWindow::resetAgentsList);

    connect(this, &CoreCtrlBridge::onConnectionStateChange,
            dynamic_cast<MainWindow*>(mw), &MainWindow::updateConnectionStateChange);

    connect(this, &CoreCtrlBridge::onRunningTasksCountChange,
            dynamic_cast<MainWindow*>(mw), &MainWindow::updateRunningTasksCount);
}

