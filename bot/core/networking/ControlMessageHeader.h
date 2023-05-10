//
// Created by alsah on 03.04.23.
//

#ifndef GREY_PARROT_BOT_CONTROL_MESSAGE_HEADER_H
#define GREY_PARROT_BOT_CONTROL_MESSAGE_HEADER_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <map>
#include <optional>
#include <memory>
#include <vector>


class ControlMessageHeader{

public:
    enum class MessageType {
        SINGLE,
        FIRST,
        NEXT,
        LAST,
    };

    explicit ControlMessageHeader(const std::vector<std::byte>& data);


    [[nodiscard]] bool isValid() const;
    [[nodiscard]] const std::string &getError() const;

    [[nodiscard]] uint16_t getHeaderSize() const;

    [[nodiscard]] uint64_t getRequestId() const;

    [[nodiscard]] MessageType getMessageType() const;

    [[nodiscard]] const std::optional<std::uint32_t> &getFullPayloadSize() const;

    [[nodiscard]] const std::optional<std::uint16_t> &getParts() const;

    [[nodiscard]] const std::optional<bool> &getAccumulate() const;

    [[nodiscard]] const std::optional<std::string> &getModuleId() const;

    [[nodiscard]] const std::optional<std::uint16_t> &getSequence() const;


private:

    bool valid = false;
    std::string error;

    // size in bytes
    const static int MAIN_SECTION_SIZE = 14;

    // header general section
    std::uint16_t headerSize{};   // (2 bytes)
    std::uint64_t requestId{};    // (8 bytes)
    MessageType messageType{};    // (1 byte)

    // 'FIRST' message additional info
    std::optional<std::uint32_t> fullPayloadSize;
    std::optional<std::uint16_t> parts;
    std::optional<bool> accumulate;

    // module identifier, must be present in 'FIRST' and 'SINGLE' msg types
    std::optional<std::string> moduleId;

    // must be present in 'CONTINUATION' message
    std::optional<std::uint16_t> sequence;


    /**
     * Tries to deserialize main section of header
     * @param data - pointer to the be beginning of main section
     * @return true if deserialized successfully, false otherwise
     */
    bool parseMainSection(const std::byte* data);

    /**
     * Tries to deserialize additional section of header
     * @param data - pointer to the be beginning of additional section (main + offset)
     * @return true if deserialized successfully, false otherwise
     */
    bool parseAdditionalSection(const std::byte* data);

    /**
     * Tries to deserialize message type (with accumulation flag)
     * @param data - last byte of the main section
     * @return true if deserialized successfully, false otherwise
     */
    bool parseMessageType(std::byte type);

};

#endif //GREY_PARROT_BOT_CONTROL_MESSAGE_HEADER_H
