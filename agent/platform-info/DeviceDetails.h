//
// Created by alsah on 14.03.23.
//

#ifndef GREY_PARROT_AGENT_DEVICE_DETAILS_H
#define GREY_PARROT_AGENT_DEVICE_DETAILS_H

#include <string>


struct DeviceDetails {
    const std::string computerId;
    const std::string computerName;

    const std::string osFullName;
};


/**
 * @see
 *     LinuxInfoCollector
 *     WinInfoCollector
 */
class IDeviceDetailsCollector{

public:
    virtual DeviceDetails getDeviceDetails() = 0;
    virtual ~IDeviceDetailsCollector() = default;

private:
    // get unique device id based on computer name, os details
    // and hardware properties (cpu); platform-depend code !
    virtual std::string retrieveComputerId() = 0;

    virtual std::string retrieveComputerName() = 0;

    // get full name of operating system
    virtual std::string retrieveOSFullName() = 0;
};




#endif //GREY_PARROT_AGENT_DEVICE_DETAILS_H
