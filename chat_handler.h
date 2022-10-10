#ifndef CHAT_CHAT_HANDLER_H
#define CHAT_CHAT_HANDLER_H

#include <windows.h>
#include "chat_packet.h"

class chat_handler {
    SOCKET socket_;

    void show_packet(const chat_packet &packet) const;

public:
    explicit chat_handler(SOCKET socket);

    void run();
};

#endif //CHAT_CHAT_HANDLER_H
