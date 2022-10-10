#include <string>
#include <stdexcept>
#include <windows.h>
#include "packet_helper.h"

// data structure for packet
// using big-endian
// 4 bytes : packet length
// 4 bytes : timestemp
// 4 bytes : name length
// n bytes : name
// m bytes : message

void packet_helper::send_packet(SOCKET socket, const chat_packet &packet) {
    std::string raw_data{};

    // packet length
    uint32_t packet_len = 4 + 4 + 4 + packet.get_name().length() + packet.get_message().length();
    packet_len = htonl(packet_len);
    raw_data.append(reinterpret_cast<const char *>(&packet_len), sizeof(packet_len));

    // timestemp
    uint32_t timestemp = htonl(packet.get_timestamp());
    raw_data.append(reinterpret_cast<const char *>(&timestemp), sizeof(timestemp));

    // name length
    uint32_t name_length = htonl(packet.get_name().length());
    raw_data.append(reinterpret_cast<const char *>(&name_length), sizeof(name_length));

    // name
    raw_data.append(packet.get_name());

    // message
    raw_data.append(packet.get_message());

    int result = send(socket, raw_data.c_str(), raw_data.length(), 0);
    if (result == SOCKET_ERROR) {
        throw std::runtime_error("send failed");
    }
}

static void read_data(SOCKET socket, char *buf, int len) {
    int read_len = 0;
    while (read_len < len) {
        int n = recv(socket, buf + read_len, len - read_len, 0);
        if (n <= 0) {
            throw std::runtime_error("connection closed");
        }
        read_len += n;
    }
}

chat_packet packet_helper::receive_packet(SOCKET socket) {
    // read packet length
    uint32_t packet_len;
    read_data(socket, reinterpret_cast<char *>(&packet_len), sizeof(packet_len));
    packet_len = ntohl(packet_len);

    // allocate packet on stack
    char buf[packet_len];

    // read full packet
    read_data(socket, buf + 4, packet_len - 4);

    // decode packet
    uint32_t timestamp = ntohl(*reinterpret_cast<uint32_t *>(buf + 4));
    uint32_t name_length = ntohl(*reinterpret_cast<uint32_t *>(buf + 8));
    std::string name{buf + 12, name_length};
    std::string message{buf + 12 + name_length, packet_len - (12 + name_length)};

    return chat_packet{timestamp, name, message};
}
