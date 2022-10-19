#include <iostream>
#include <thread>
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

    // create a thread to accept commands
    std::thread thread_cmd([s, this]{
        std::string cmd;
        while(true) {
            std::cout << " (SERVER)> " << std::flush;
            std::cin >> cmd;
            if (cmd == "!exit") {
                closesocket(s);
                {
                    std::lock_guard lock(m_);
                    for (auto each_client : clients_) {
                        closesocket(each_client);
                    }
                }
                break;
            } else {
                std::cout << "unknown command" << std::endl;
            }
        }
    });

    sockaddr_in clientAddr{0};
    int clientAddrLen = sizeof(clientAddr);
    while (true) {
        SOCKET client = accept(s, (sockaddr *) &clientAddr, &clientAddrLen);
        if (client == INVALID_SOCKET) {
            break;
        }

        // create a new thread to handle this connection
        std::thread t([client, this]{
            // add the new connection to the client set
            {
                std::lock_guard lock(m_);
                clients_.insert(client);
            }

            // handle the connection
            char buf[512];
            int n;
            // if the n <= 0, the connection is dead
            while((n = recv(client, buf, sizeof(buf), 0)) >= 0) {
                std::lock_guard lock(m_);
                for (auto each_client : clients_) {
                    if (each_client == client) {
                        continue;
                    }

                    // try to send the received msg to other clients
                    send(each_client, buf, n, 0);
                }
            }

            // remove the connection from the client list
            {
                std::lock_guard lock(m_);
                clients_.erase(client);
            }

            closesocket(client);
        });
        t.detach();
    }

    // waiting for the command thread to exit
    thread_cmd.join();
}
