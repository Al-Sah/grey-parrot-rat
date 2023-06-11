//
// Created by alsah on 10.06.23.
//

#ifndef GREY_PARROT_RAT_ECHO_MODULE_ECHO_MAKER_H
#define GREY_PARROT_RAT_ECHO_MODULE_ECHO_MAKER_H


#include "TaskGenerator.h"
#include "EchoModuleBridge.h"
#include <QWidget>

class EchoMaker : public TaskGenerator{
public:

    EchoMaker();
    virtual ~EchoMaker();
    QWidget *getUI(QWidget *parent) override;

    void handleResult(Task task) override;
    void requestDataChannel();


    void sendText(const std::string &data);
    void sendFile(const std::string &filepath);

private:

    void setDataChannelHandlers() override;

    QWidget* moduleUi{nullptr};
    EchoModuleBridge* bridge{nullptr};
};


#endif //GREY_PARROT_RAT_ECHO_MODULE_ECHO_MAKER_H
