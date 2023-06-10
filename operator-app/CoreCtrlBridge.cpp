//
// Created by alsah on 06.06.23.
//

#include "CoreCtrlBridge.h"

#include "gui/MainWindow.h"

CoreCtrlBridge::CoreCtrlBridge(QWidget *target) {

    qRegisterMetaType<msgs::ActiveAgents>("ActiveAgents");
    qRegisterMetaType<msgs::AgentDescription>("AgentDescription");
    qRegisterMetaType<ConnectionChange>("ConnectionChange");
    qRegisterMetaType<std::string>("stdString");

    auto* mw = dynamic_cast<MainWindow*>(target);

    connect(this, &CoreCtrlBridge::onActiveAgents, mw, &MainWindow::resetAgentsList);
    connect(this, &CoreCtrlBridge::onNewAgent, mw, &MainWindow::addNewAgent);
    connect(this, &CoreCtrlBridge::onAgentDisconnect, mw, &MainWindow::removeAgent);

    connect(this, &CoreCtrlBridge::onC2ServerConnectionState, mw, &MainWindow::updateC2ServerConnectionState);
    connect(this, &CoreCtrlBridge::onPeerConnectionState, mw, &MainWindow::updatePeerConnectionState);
    connect(this, &CoreCtrlBridge::onRunningTasksCountChange, mw, &MainWindow::updateRunningTasksCount);
}

