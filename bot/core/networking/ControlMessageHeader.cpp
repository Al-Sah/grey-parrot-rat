//
// Created by alsah on 03.04.23.
//

#include <cstring>
#include <iostream>
#include "ControlMessageHeader.h"

#include <endian.h>


// Utils functions
namespace parse{

    static std::uint32_t get16uint(const std::byte* data){
        std::uint16_t res;
        std::memcpy(&res, data, sizeof(std::uint16_t));
        return be16toh(res);
    }
    static std::uint32_t get32uint(const std::byte* data){
        std::uint32_t res;
        std::memcpy(&res, data, sizeof(std::uint32_t));
        return be32toh(res);
    }
    static std::uint32_t get64uint(const std::byte* data){
        std::uint64_t res;
        std::memcpy(&res, data, sizeof(std::uint64_t));
        return be64toh(res);
    }

}



ControlMessageHeader::ControlMessageHeader(const std::vector<std::byte>& message) {

    try {

        if(!parseMainSection(message.data())){
            // failed to deserialize main section - not reasonable to continue
            return;
        }
        if(parseAdditionalSection(message.data() + MAIN_SECTION_SIZE)){
            valid = true;
        }
    } catch (...){
        error = "unexpected deserialization error";
    }
}

bool ControlMessageHeader::parseMainSection(const std::byte* data) {

    // /- - - - - - - -MAIN SECTION - - - - - - - -\
    // | 3 bytes | 2 bytes |  8 bytes   |  1 byte  |
    // |  MAGIC  | H. SIZE | REQUEST ID | MSG TYPE |  <- 14 bytes in total

    const char magic[3] = { 'R', 'C', 'M' };
    if (std::memcmp(magic, data, sizeof(magic)) != 0) {
        error = "invalid main section: unexpected magic bytes";
        return false;
    }

    headerSize = parse::get16uint(data + 3); // 3 bytes - 'header size' field offset
    if (headerSize < MAIN_SECTION_SIZE) {
        error = "invalid main section: unexpected header size - " + std::to_string(headerSize);
        return false;
    }

    requestId = parse::get64uint(data + 5);  // 5 bytes - 'request id' field offset

    return parseMessageType(data[13]); // last byte of main section
}

bool ControlMessageHeader::parseMessageType(std::byte type) {
    switch (type) {
        case std::byte{0}:
            messageType = MessageType::SINGLE;
            break;
        case std::byte{1}:
            messageType = MessageType::FIRST;
            accumulate = false;
            break;
        case std::byte{2}:
            messageType = MessageType::NEXT;
            break;
        case std::byte{3}:
            messageType = MessageType::LAST;
            break;
        case std::byte{5}:
            messageType = MessageType::FIRST;
            accumulate = true;
            break;
        default: {
            error = "invalid main section: unexpected message type - " +
                    std::to_string(static_cast<std::uint16_t>(type));
            return false;
        }
    }
    return true;
}

bool ControlMessageHeader::parseAdditionalSection(const std::byte *data) {

    switch (messageType) {
        case MessageType::SINGLE:
            moduleId = std::string(reinterpret_cast<const char *>(data+1), static_cast<size_t>(data[0]));
            if(!moduleId.has_value() || moduleId->empty() || moduleId->size() > 255){
                error = "invalid additional section: invalid module id";
                return false;
            }
            break;
        case MessageType::FIRST:
            parts = parse::get16uint(data);
            fullPayloadSize = parse::get32uint(data + 2);
            moduleId = std::string(reinterpret_cast<const char *>(data+7), static_cast<size_t>(data[6]));
            if(!fullPayloadSize.has_value() || fullPayloadSize == 0){
                error = "invalid additional section: invalid payload size";
                return false;
            }
            if(!moduleId.has_value() || moduleId->empty() || moduleId->size() > 255){
                error = "invalid additional section: invalid module id";
                return false;
            }
            break;
        case MessageType::NEXT:
            sequence = parse::get16uint(data);
            break;
        case MessageType::LAST:
            // 'LAST' message does not have additional section
            break;
    }
    return true;
}

bool ControlMessageHeader::isValid() const {
    return valid;
}

const std::string &ControlMessageHeader::getError() const {
    return error;
}
