//
// Created by alsah on 12.06.23.
//

#ifndef GREY_PARROT_RAT_FILESMANAGEROPERATORAPP_H
#define GREY_PARROT_RAT_FILESMANAGEROPERATORAPP_H


#include "TaskGenerator.h"

class FilesManagerOperatorApp : public TaskGenerator{

public:
    FilesManagerOperatorApp();
    void handleResult(Task task) override;

    QWidget *getUI(QWidget *parent) override;

protected:
    QWidget* moduleUi{nullptr};
    void setDataChannelHandlers() override;
};


#endif //GREY_PARROT_RAT_FILESMANAGEROPERATORAPP_H
