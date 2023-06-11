//
// Created by alsah on 10.05.23.
//

#ifndef GREY_PARROT_RAT_SDK_TASK_H
#define GREY_PARROT_RAT_SDK_TASK_H

#include <string>
#include <optional>
#include <vector>
#include <variant>

/**
 *  Data structure that describes "Task".
 *  Can be used to create "Request" as well as "Response"
 */
struct Task {
    /// task identifier (uuid)
    const std::uint64_t id;
    /// module identifier (name)
    const std::string module;
    /// payload (binary data can be passed as string or vector)
    const std::variant<std::vector<std::byte>, std::string> payload;

    // payload can be present in ram (binary form or common string)
    // or as file. In case if "isFilepath" is true, payload contains filepath
    /// Type of payload
    const bool isFilepath;
    /// Indicates whether this 'Task' is last
    const bool isClosing;

    // Task can be passed by parts
    /// Indicates whether message it is just part of message
    const bool isPartial;
    /// Current part (sequence number)
    const std::optional<std::uint16_t> part;
    /// Number of parts
    const std::optional<std::uint16_t> parts;
    /// Full size of payload (all partitions)
    const std::optional<std::uint32_t> size;

    const bool asPeer;
};

#endif //GREY_PARROT_RAT_SDK_TASK_H
