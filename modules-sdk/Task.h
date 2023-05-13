//
// Created by alsah on 10.05.23.
//

#ifndef GREY_PARROT_SDK_TASK_MODEL_H
#define GREY_PARROT_SDK_TASK_MODEL_H

#include <string>
#include <optional>
#include <vector>

struct Task {
    const std::uint64_t task_id;
    const std::string module_id;

    // payload can be represented in binary form or as file
    const bool inMemory;
    const std::optional<std::string> filepath;
    const std::optional<std::vector<std::byte>> payload;

    // some modules can hold partial messages ...
    // if message is partial, set part (sequence) number
    const bool isPartial;
    const std::optional<std::uint16_t> part;
    const std::optional<std::uint16_t> parts;
    const std::optional<std::uint32_t> size;
};


#endif //GREY_PARROT_SDK_TASK_MODEL_H
