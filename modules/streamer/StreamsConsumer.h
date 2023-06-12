//
// Created by alsah on 11.06.23.
//

#ifndef GREY_PARROT_RAT_STREAMSCONSUMER_H
#define GREY_PARROT_RAT_STREAMSCONSUMER_H


#include "TaskGenerator.h"

class StreamsConsumer : public TaskGenerator{

public:
    StreamsConsumer();
    void handleResult(Task task) override;

    QWidget *getUI(QWidget *parent) override;

protected:
    QWidget* moduleUi{nullptr};
    void setDataChannelHandlers() override;
};


#endif //GREY_PARROT_RAT_STREAMSCONSUMER_H
