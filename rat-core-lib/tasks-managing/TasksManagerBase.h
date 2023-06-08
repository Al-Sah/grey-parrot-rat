//
// Created by alsah on 08.05.23.
//

#ifndef GREY_PARROT_RAT_CORE_TASKS_MANAGER_H
#define GREY_PARROT_RAT_CORE_TASKS_MANAGER_H


#include "TasksMap.h"


#include "IControlPacketHandler.h"
#include "IControlPacketSender.h"
#include "ITaskHandler.h"


class TasksManagerBase : public IControlPacketHandler, public ITaskHandler{

public:

    void setMessagesSender(const std::shared_ptr<IControlPacketSender> &aMessagesSender);
    void setTaskHandler(const std::shared_ptr<ITaskHandler> &aTaskHandler);

    virtual ~TasksManagerBase() = default;

    [[nodiscard]] TasksMap &getTasksMap();

    void setOnTasksCountChange(const std::function<void(std::uint32_t)> &onTasksCountChange);

protected:

    TasksMap tasks;

    std::shared_ptr<IControlPacketSender> messagesSender;
    std::shared_ptr<ITaskHandler> taskHandler;

    std::function<void(std::uint32_t)> onTasksCountChange;
};

#endif //GREY_PARROT_RAT_CORE_TASKS_MANAGER_H
