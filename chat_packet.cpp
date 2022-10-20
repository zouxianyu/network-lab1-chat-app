#include <cstdint>
#include <utility>
#include "chat_packet.h"

chat_packet::chat_packet(
        uint32_t timestamp,
        std::string name,
        std::string message
) : timestamp_(timestamp),
    name_(std::move(name)),
    message_(std::move(message)) {}

uint32_t chat_packet::get_timestamp() const {
    return timestamp_;
}

std::string chat_packet::get_name() const {
    return name_;
}

std::string chat_packet::get_message() const {
    return message_;
}

