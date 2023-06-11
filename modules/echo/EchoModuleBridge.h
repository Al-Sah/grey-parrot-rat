//
// Created by alsah on 10.06.23.
//

#ifndef GREY_PARROT_RAT_MODULE_ECHO_MODULE_BRIDGE_H
#define GREY_PARROT_RAT_MODULE_ECHO_MODULE_BRIDGE_H


#include <QWidget>


class EchoModuleBridge : public QWidget{

    Q_OBJECT

public:
    explicit EchoModuleBridge(QWidget* target);

    signals:

    void onText(std::string);
    void onFile(std::string);
    void onDataChannel(bool);
    void onDataChannelError(std::string);

};


#endif //GREY_PARROT_RAT_MODULE_ECHO_MODULE_BRIDGE_H
