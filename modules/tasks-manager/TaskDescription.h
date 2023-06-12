//
// Created by alsah on 12.06.23.
//

#ifndef GREY_PARROT_RAT_TASKDESCRIPTION_H
#define GREY_PARROT_RAT_TASKDESCRIPTION_H

#include <string>

struct TaskDescription{

    const std::string programName;
    const std::int32_t taskId;
    const int parentProcId;
    const std::uint64_t startTime;

    const std::uint64_t realPriority;

    const std::uint64_t vmSize;

    const std::string effectiveUserName;
    const std::string realUserName;
    const std::string savedUserName;
    const std::string filesystemUserName;

    const std::string effectiveGroupName;
    const std::string realGroupName;
    const std::string savedGroupName;
    const std::string filesystemGroupName;

};



#endif //GREY_PARROT_RAT_TASKDESCRIPTION_H
