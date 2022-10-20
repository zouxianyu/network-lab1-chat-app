#include <iostream>
#include <utility>
#include <windows.h>
#include "chat_handler.h"
#include "client_helper.h"

client_helper::client_helper(std::string ip, int port)
        : ip_(std::move(ip)), port_(port) {}

void client_helper::run() {
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        return;
    }
    sockaddr_in addr{0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = inet_addr(ip_.c_str());
    int result = connect(s, (sockaddr *) &addr, sizeof(addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        return;
    }

    std::cout << "connected" << std::endl;
    chat_handler handler(s);
    handler.run();
}
