#ifndef CHAT_PACKET_HELPER_H
#define CHAT_PACKET_HELPER_H

#include <windows.h>
#include "chat_packet.h"

namespace packet_helper {
    void send_packet(SOCKET socket, const chat_packet &packet);
    chat_packet receive_packet(SOCKET socket);
}
#endif //CHAT_PACKET_HELPER_H
