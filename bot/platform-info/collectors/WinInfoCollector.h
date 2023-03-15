//
// Created by alsah on 14.03.23.
//

#ifndef GREY_PARROT_BOT_WINDOWS_INFO_COLLECTOR_H
#define GREY_PARROT_BOT_WINDOWS_INFO_COLLECTOR_H

#include <windows.h>
#include "sysinfoapi.h"

#include "../DeviceDetails.h"

class WinInfoCollector final : public IDeviceDetailsCollector{

public:
    DeviceDetails getDeviceDetails() override;
    WinInfoCollector();

private:
    OSVERSIONINFOEX osVersionInfo{};
    std::int32_t getVersionResCode;
    std::string computerName;

    std::string retrieveComputerId() override;
    std::string retrieveComputerName() override;
    std::string retrieveOSFullName() override;

};


#endif //GREY_PARROT_BOT_WINDOWS_INFO_COLLECTOR_H
