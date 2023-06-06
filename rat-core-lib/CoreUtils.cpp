//
// Created by alsah on 05.06.23.
//

#include <algorithm>
#include "CoreUtils.h"

std::string CoreUtils::convert(ChannelState state) {
    static std::map<ChannelState,std::string> values{
            {ChannelState::INITIALIZED, "ready"},
            {ChannelState::OPENING, "opening"},
            {ChannelState::ERROR_CAUGHT, "caught error"},
            {ChannelState::OPENED, "opened"},
            {ChannelState::WAITING_TO_RECOVER, "waiting to recover"},
            {ChannelState::CLOSED, "closed"},
    };

    return values[state];
}

std::vector<std::byte> CoreUtils::convert(const std::string &str) {

    std::vector<std::byte> bytes;
    bytes.reserve(str.size());

    std::transform(
            std::begin(str),
            std::end(str),
            std::back_inserter(bytes),
            [](char byteChar){ return std::byte(byteChar);
            });

    return bytes;
}
