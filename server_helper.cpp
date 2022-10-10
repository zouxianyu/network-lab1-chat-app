#include <iostream>
#include <windows.h>
#include "chat_handler.h"
#include "server_helper.h"

server_helper::server_helper(int port)
        : port_(port) {}

void server_helper::run() {
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        return;
    }
    sockaddr_in addr{0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int result = bind(s, (sockaddr *) &addr, sizeof(addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        return;
    }

    result = listen(s, 1);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        return;
    }

    sockaddr_in clientAddr{0};
    int clientAddrLen = sizeof(clientAddr);
    SOCKET client = accept(s, (sockaddr *) &clientAddr, &clientAddrLen);
    if (client == INVALID_SOCKET) {
        std::cerr << "Error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        return;
    }

    std::cout << "connected" << std::endl;
    chat_handler handler(client);
    handler.run();

    closesocket(s);
}
