//
// Created by alsah on 14.03.23.
//

#include <iostream>
#include <map>
#include "WinInfoCollector.h"
#include "winbase.h"
#include <intrin.h>

// https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-osversioninfoexa

static const std::map<std::string, std::string> windowsWorkstations = {
        {"10.0", "Windows 10"},
        {"6.3", "Windows 8.1"},
        {"6.2", "Windows 8"},
        {"6.1", "Windows 7"},
        //{"6.0", "Windows Vista"},
        // Older versions are not supported
};
static const std::map<std::string, std::string> windowsServers = {
        {"10.0", "Windows Server 2016"},
        {"6.3", "Windows Server 2012 R2"},
        {"6.2", "Windows Server 2012"},
        //{"6.1", "Windows Server 2008 R2"},
        //{"6.0", "Windows Server 2008"},
        // Older versions are not supported
};


DeviceDetails WinInfoCollector::getDeviceDetails() {

    return {
            .computerId = retrieveComputerId(),
            .computerName = computerName,
            .osFullName = retrieveOSFullName(),
    };
}

std::string WinInfoCollector::retrieveComputerId() {

    int cpuinfo[4] = { 0, 0, 0, 0 };
    __cpuid( cpuinfo, 0 );

    // some magic from stackoverflow
    std::uint16_t hash = 0;
    auto* ptr = (std::uint16_t*)(&cpuinfo[0]);
    for (std::uint16_t i = 0; i < 8; i++ ){
        hash += ptr[i];
    }

    return std::to_string(std::hash<std::string>()(computerName + std::to_string(hash)));
}

WinInfoCollector::WinInfoCollector() {

    ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    getVersionResCode = GetVersionEx((LPOSVERSIONINFO)&osVersionInfo);

    computerName = retrieveComputerName();
}

std::string WinInfoCollector::retrieveOSFullName() {
    if(getVersionResCode == 0){
        return "Windows undefined";
    }

    std::string version = std::to_string(osVersionInfo.dwMajorVersion) + '.'
            + std::to_string(osVersionInfo.dwMinorVersion);

    if (osVersionInfo.wProductType == VER_NT_WORKSTATION){

        auto iterator = windowsWorkstations.find(version);
        if (iterator != windowsWorkstations.end()) {
            return iterator->second;
        }

    } else {
        auto iterator = windowsServers.find(version);
        if (iterator != windowsServers.end()) {
            return iterator->second;
        }
    }
    return "Windows undefined";
}

std::string WinInfoCollector::retrieveComputerName() {
    TCHAR infoBuf[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD bufCharCount = MAX_COMPUTERNAME_LENGTH + 1;

    if(GetComputerName( infoBuf, &bufCharCount) != 0){
       return std::string(infoBuf);
    }
    return "undefined";
}
