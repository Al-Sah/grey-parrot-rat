//
// Created by alsah on 05.06.23.
//

#ifndef GREY_PARROT_RAT_CORE_UTILS_H
#define GREY_PARROT_RAT_CORE_UTILS_H


#include <string>
#include <map>
#include <vector>
#include "networking/ChannelState.h"

class CoreUtils {

public:

    static std::string convert(ChannelState state);

    static std::vector<std::byte> convert(const std::string& str);
};


#endif //GREY_PARROT_RAT_CORE_UTILS_H
