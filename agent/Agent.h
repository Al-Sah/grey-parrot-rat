//
// Created by alsah on 11.03.23.
//

#ifndef GREY_PARROT_AGENT_H
#define GREY_PARROT_AGENT_H

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "platform-info/DeviceDetails.h"
#include "app-info/ApplicationDetails.h"

#include "core-data.pb.h"

#include "BS_thread_pool.hpp"

#include "tasks-managing/TasksManagerBase.h"
#include "networking/ConnectionsManager.h"
#include "core/AgentModulesManager.h"
#include "core/AgentTasksManager.h"

#include <TaskExecutor.h>



class Agent : public TaskExecutor {

public:
    //singleton pattern
    Agent(Agent &other) = delete;
    void operator=(const Agent &) = delete;

    static Agent *GetInstance();
    int runPerpetual();

private:

    void execute(Task task) override;

    static volatile bool run;
    static std::mutex mutex;
    static std::condition_variable run_cv;

    static BS::thread_pool pool;

    static Agent* bot;
    explicit Agent(std::unique_ptr<IDeviceDetailsCollector> infoCollector);

    std::shared_ptr<ConnectionsManager> connectionsManager;
    std::shared_ptr<AgentTasksManager> tasksManager;
    std::shared_ptr<AgentModulesManager> modulesManager;


    DeviceDetails deviceDetails;
    ApplicationDetails applicationDetails;

    static void handleSystemSignal(int signal);

    msgs::AgentDescription to_proto_message();

protected:
    void setDataChannelHandlers() override;
};


#endif //GREY_PARROT_AGENT_H
