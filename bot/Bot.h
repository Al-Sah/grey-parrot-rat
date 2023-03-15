//
// Created by alsah on 11.03.23.
//

#ifndef GREY_PARROT_BOT_H
#define GREY_PARROT_BOT_H

#include <string>
#include <memory>

#include "platform-info/DeviceDetails.h"
#include "app-info/ApplicationDetails.h"

class Bot {

public:
    //singleton pattern
    Bot(Bot &other) = delete;
    void operator=(const Bot &) = delete;

    static Bot *GetInstance();
    static int runPerpetual();

private:
    static volatile bool run;

    static Bot* bot;
    explicit Bot(std::unique_ptr<IDeviceDetailsCollector> infoCollector);

    DeviceDetails deviceDetails;
    ApplicationDetails applicationDetails;

    static void handleSystemSignal(int signal);
};


#endif //GREY_PARROT_BOT_H
