//
// Created by alsah on 14.03.23.
//

#include <chrono>
#include "ApplicationDetails.h"

bool ApplicationDetails::checkIsRoot() {
    // TODO: implement this check;
    //  try access some recurse that require root privileges
    return false;
}


ApplicationDetails::ApplicationDetails() :
        isRoot(ApplicationDetails::checkIsRoot()),
        startTime(std::chrono::system_clock::now().time_since_epoch().count()) {}
