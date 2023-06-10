//
// Created by alsah on 06.06.23.
//

#ifndef GREY_PARROT_RAT_OPERATOR_CORE_CTRL_BRIDGE_H
#define GREY_PARROT_RAT_OPERATOR_CORE_CTRL_BRIDGE_H

#include <QWidget>
#include "core-data.pb.h"
#include "ConnectionChangeInfo.h"

Q_DECLARE_METATYPE(msgs::ActiveAgents)
Q_DECLARE_METATYPE(msgs::AgentDescription)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(ConnectionChange)

class CoreCtrlBridge : public QWidget{

    Q_OBJECT

public:
    explicit CoreCtrlBridge(QWidget* target);

signals:
    void onActiveAgents(const msgs::ActiveAgents& agents);
    void onNewAgent(const msgs::AgentDescription& agentDescription);
    void onAgentDisconnect(const std::string& agentId);
    void onRunningTasksCountChange(int count);

    void onC2ServerConnectionState(const ConnectionChange& change);
    void onPeerConnectionState(const ConnectionChange& change);

};

#endif //GREY_PARROT_RAT_OPERATOR_CORE_CTRL_BRIDGE_H
