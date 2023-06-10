//
// Created by alsah on 09.06.23.
//

#ifndef GREY_PARROT_RAT_OPERATOR_CONNECTION_CHANGE_INFO_H
#define GREY_PARROT_RAT_OPERATOR_CONNECTION_CHANGE_INFO_H

#include <string>

struct ConnectionChange{
    bool opened;
    std::string state;
    std::uint64_t timestamp;
};

#endif //GREY_PARROT_RAT_OPERATOR_CONNECTION_CHANGE_INFO_H
