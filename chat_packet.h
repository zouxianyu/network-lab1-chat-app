#ifndef CHAT_CHAT_PACKET_H
#define CHAT_CHAT_PACKET_H

#include <string>
#include <cstdint>

class chat_packet {
    uint32_t timestamp_;
    std::string name_;
    std::string message_;

public:
    chat_packet(
            uint32_t timestamp,
            const std::string &name,
            const std::string &message
    );

    uint32_t get_timestamp() const;

    std::string get_name() const;

    std::string get_message() const;
};

#endif //CHAT_CHAT_PACKET_H
