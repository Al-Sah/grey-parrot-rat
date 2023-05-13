//
// Created by alsah on 11.03.23.
//

#ifndef GREY_PARROT_BOT_H
#define GREY_PARROT_BOT_H

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "platform-info/DeviceDetails.h"
#include "app-info/ApplicationDetails.h"

#include "core/networking/ConnectionsManager.h"
#include "core/BS_thread_pool.hpp"
#include "core/tasks-managing/TasksManager.h"

#include "../modules-sdk/Module.h"

class Bot {

public:
    //singleton pattern
    Bot(Bot &other) = delete;
    void operator=(const Bot &) = delete;

    static Bot *GetInstance();
    static int runPerpetual();

private:
    static volatile bool run;
    static std::mutex mutex;
    static std::condition_variable run_cv;

    static BS::thread_pool pool;

    static Bot* bot;
    explicit Bot(std::unique_ptr<IDeviceDetailsCollector> infoCollector);

    std::shared_ptr<ConnectionsManager> connectionsManager;
    std::shared_ptr<TasksManager> tasksManager;


    DeviceDetails deviceDetails;
    ApplicationDetails applicationDetails;

    static void handleSystemSignal(int signal);
};


#endif //GREY_PARROT_BOT_H
