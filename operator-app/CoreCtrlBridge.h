//
// Created by alsah on 06.06.23.
//

#ifndef GREY_PARROT_RAT_OPERATOR_CORE_CTRL_BRIDGE_H
#define GREY_PARROT_RAT_OPERATOR_CORE_CTRL_BRIDGE_H

#include <QWidget>
#include "core-data.pb.h"

Q_DECLARE_METATYPE(msgs::ActiveAgents)
Q_DECLARE_METATYPE(std::string)

class CoreCtrlBridge : public QWidget{

    Q_OBJECT

public:
    explicit CoreCtrlBridge(QWidget* target);

signals:
    void onActiveAgents(const msgs::ActiveAgents& agents);
    void onConnectionStateChange(bool opened, const std::string& state, unsigned long time);
};

#endif //GREY_PARROT_RAT_OPERATOR_CORE_CTRL_BRIDGE_H
