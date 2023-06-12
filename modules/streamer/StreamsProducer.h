//
// Created by alsah on 11.06.23.
//

#ifndef GREY_PARROT_RAT_STREAMSPRODUCER_H
#define GREY_PARROT_RAT_STREAMSPRODUCER_H

#include "TaskExecutor.h"

class StreamsProducer : public TaskExecutor{

public:
    StreamsProducer();
    void execute(Task task) override;

protected:
    void setDataChannelHandlers() override;
};


#endif //GREY_PARROT_RAT_STREAMSPRODUCER_H
