//
// Created by alsah on 11.03.23.
//

#ifndef GREY_PARROT_BOT_H
#define GREY_PARROT_BOT_H

#include <string>

class Bot {

public:
    //singleton pattern
    Bot(Bot &other) = delete;
    void operator=(const Bot &) = delete;
    static Bot *GetInstance();

    int runPerpetual();

private:
    static volatile bool run;
    static Bot* bot;

    const std::string computerId;
    const std::string botVersion;

    static void handleSystemSignal(int signal);
    Bot();

    static std::string retrieveComputerId();
};


#endif //GREY_PARROT_BOT_H
