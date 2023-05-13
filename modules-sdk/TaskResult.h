//
// Created by alsah on 10.05.23.
//

#ifndef GREY_PARROT_CLIENTS_TASKRESULT_H
#define GREY_PARROT_CLIENTS_TASKRESULT_H


#include <string>
#include <optional>
#include <vector>

struct TaskResult {

    const bool isClosing;

    const std::string module_id;
    const std::string task_id;

    // payload can be represented in binary form or as file
    const std::optional<std::string> filepath;
    const std::optional<std::vector<std::byte>> payload;

};


#endif //GREY_PARROT_CLIENTS_TASKRESULT_H
