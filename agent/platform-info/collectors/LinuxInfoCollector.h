//
// Created by alsah on 14.03.23.
//

#ifndef GREY_PARROT_AGENT_LINUX_INFO_COLLECTOR_H
#define GREY_PARROT_AGENT_LINUX_INFO_COLLECTOR_H


#include <sys/utsname.h>
#include "../DeviceDetails.h"

class LinuxInfoCollector final : public IDeviceDetailsCollector{

public:
    DeviceDetails getDeviceDetails() override;
    LinuxInfoCollector();

private:

    utsname unameData{};
    int32_t unameResCode;
    std::string computerName;

    std::string retrieveComputerName() override;
    std::string retrieveComputerId() override;
    std::string retrieveOSFullName() override;
};


#endif //GREY_PARROT_AGENT_LINUX_INFO_COLLECTOR_H
