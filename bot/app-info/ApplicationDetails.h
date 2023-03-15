//
// Created by alsah on 14.03.23.
//

#ifndef GREY_PARROT_BOT_APPLICATION_DETAILS_H
#define GREY_PARROT_BOT_APPLICATION_DETAILS_H

#include <cstdint>
#include <string>

#include "bot-version.h"
#include "platform.h"


struct ApplicationDetails {

    // *** Runtime configuration ****
    /// sudo user (linux) / Administrator (Windows)
    const bool isRoot;
    // TODO: check if const. Potentially can be changed in runtime ??

    /// UNIX timestamp; time since epoch
    const std::uint64_t startTime;

    // TODO: add running as (by user/as a service/...)

    // *** Compilation time configuration ***
    /// Version as "MAJOR.MINOR.PATCH"
    const std::string botVersion = GREY_PARROT_BOT_VERSION;

    // Application could be possibly run in WSL or with wine ???
    /// Destination platform (Linux or Windows)
    const std::string compiledFor = PLATFORM_NAME;

    /// Compiled as 32x or 64x executable ( 0 if something else)
    const std::uint8_t compiledForArc = PLATFORM_ARCHITECTURE;

    ApplicationDetails();
    static bool checkIsRoot();
};

#endif //GREY_PARROT_BOT_APPLICATION_DETAILS_H