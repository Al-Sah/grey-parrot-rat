//
// Created by alsah on 14.03.23.
//

#include "LinuxInfoCollector.h"

#include <sstream>
#include <fstream>

#include <unistd.h> // gethostname function
#include <climits>  // HOST_NAME_MAX
#include <iostream>


/**
 * Read file and find line that starts with specified substring.
 *
 * @param filename file to open
 * @param substring substring that must be present in line
 * @return line from file or empty string if not found
 */
static std::string findLineInFile(const std::string& filename, const std::string& substring){
    std::ifstream file (filename);
    std::string fileLine;
    if (file.is_open()) {
        while(std::getline(file, fileLine)){
            if(fileLine.starts_with(substring)){
                break;
            }
        }
        file.close();
    } else{
        std::cout << "Failed to open lsb-release"; // TODO: add logger
    }

    return fileLine;
}



LinuxInfoCollector::LinuxInfoCollector() {
    unameResCode = uname(&unameData);
    computerName = retrieveComputerName();
}

std::string LinuxInfoCollector::retrieveComputerId() {

    std::stringstream ss;
    ss << computerName;

    if(unameResCode == 0){
        ss << unameData.sysname << unameData.machine;
    }

    std::ifstream cpuinfo ("/proc/cpuinfo");
    if (cpuinfo.is_open()) {

        // read just first 4 lines of cpuinfo (starting from second); it contains:
        // vendor_id, cpu family, model, model name
        std::string fileLine;
        for (int lines = 1; lines < 5 && std::getline(cpuinfo, fileLine); ++lines){
            ss << fileLine;
        }
        cpuinfo.close();
    }
    // TODO get serial number ? (Just right now it is probably enough to use nodename + cpuinfo )
    return std::to_string(std::hash<std::string>()(ss.str()));
}


DeviceDetails LinuxInfoCollector::getDeviceDetails() {
    return DeviceDetails{
            .computerId = retrieveComputerId(),
            .computerName = computerName,
            .osFullName =  retrieveOSFullName(),
    };
}

std::string LinuxInfoCollector::retrieveComputerName() {

    if(unameResCode == 0){
        return std::string(unameData.nodename);
    }

    char hostname[HOST_NAME_MAX + 1];
    if(gethostname(hostname, HOST_NAME_MAX + 1) == 0){
        return std::string(hostname);
    }

    return "undefined";
}

std::string LinuxInfoCollector::retrieveOSFullName() {

    static const std::string key1 = "DISTRIB_DESCRIPTION=";
    static const std::string key2 = "PRETTY_NAME=";

    std::string result = findLineInFile("/etc/lsb-release", key1);
    if(!result.empty()){
        result = result.substr(key1.size());
        result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
        return result;
    }

    result = findLineInFile("/etc/os-release", key2);
    if(!result.empty()){
        result = result.substr(key2.size());
        result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
        return result;
    }

    return "Linux undefined";
}
