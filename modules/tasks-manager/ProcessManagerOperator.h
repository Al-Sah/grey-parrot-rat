//
// Created by alsah on 12.06.23.
//

#ifndef GREY_PARROT_RAT_PROCESSMANAGEROPERATOR_H
#define GREY_PARROT_RAT_PROCESSMANAGEROPERATOR_H


#include "TaskGenerator.h"

class ProcessManagerOperator : public TaskGenerator{

public:
    ProcessManagerOperator();
    void handleResult(Task task) override;

    QWidget *getUI(QWidget *parent) override;

protected:
    QWidget* moduleUi{nullptr};
    void setDataChannelHandlers() override;
};


#endif //GREY_PARROT_RAT_PROCESSMANAGEROPERATOR_H
