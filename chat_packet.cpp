#include <cstdint>
#include "chat_packet.h"

chat_packet::chat_packet(
        uint32_t timestamp,
        const std::string &name,
        const std::string &message
) : timestamp_(timestamp),
    name_(name),
    message_(message) {}

uint32_t chat_packet::get_timestamp() const {
    return timestamp_;
}

std::string chat_packet::get_name() const {
    return name_;
}

std::string chat_packet::get_message() const {
    return message_;
}

