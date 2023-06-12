//
// Created by alsah on 12.06.23.
//

#ifndef GREY_PARROT_RAT_FILESMANAGERAGENT_H
#define GREY_PARROT_RAT_FILESMANAGERAGENT_H


#include "TaskExecutor.h"

class FilesManagerAgent : public TaskExecutor{

public:
    FilesManagerAgent();
    void execute(Task task) override;

protected:
    void setDataChannelHandlers() override;
};


#endif //GREY_PARROT_RAT_FILESMANAGERAGENT_H
