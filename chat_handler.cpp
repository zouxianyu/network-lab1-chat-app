#include <thread>
#include <iostream>
#include <ctime>
#include <windows.h>
#include "chat_packet.h"
#include "packet_helper.h"
#include "chat_handler.h"

// global name variable in main.cpp
extern std::string g_name;

chat_handler::chat_handler(SOCKET socket)
        : socket_(socket) {}

void chat_handler::show_packet(const chat_packet &packet) const {
    // get time from packet
    uint32_t timestamp = packet.get_timestamp();
    time_t timestamp64 = timestamp;
    tm timeinfo{};
    localtime_s(&timeinfo, &timestamp64);
    char timeinfo_str[80];
    strftime(
            timeinfo_str,
            sizeof(timeinfo_str),
            "%Y-%m-%d %H:%M:%S",
            &timeinfo
    );

    // show message
    std::cout << packet.get_name() << " " << "(" << timeinfo_str << "):" << std::endl;
    std::cout << packet.get_message() << std::endl;
    std::cout << std::endl;
}

void chat_handler::show_input_cmd() const {
    std::cout << g_name << " (CHAT)> ";
}

void chat_handler::run() {
    std::thread recv_thread([this] {
        try {
            while (true) {
                // get packet
                chat_packet packet = packet_helper::receive_packet(socket_);

                // handle the packet
                // clear the current line and show the packet
                std::cout << "\r\033[K";
                show_packet(packet);
                // show the input command again
                show_input_cmd();
            }
        } catch (const std::exception &e) {
//            std::cerr << e.what() << std::endl;
        }
    });

    try {
        while (true) {
            // show > at first
            show_input_cmd();

            // get message
            std::string message;
            do {
                std::getline(std::cin, message);
            } while(message.empty());

            // check if exit
            if (message == "!exit") {
                closesocket(socket_);
                break;
            }

            // get current time
            time_t timestamp = time(nullptr);

            // send packet
            chat_packet packet(
                    timestamp,
                    g_name,
                    message
            );
            packet_helper::send_packet(socket_, packet);

            // move up the cursor and show the current message
            std::cout << "\033[1A\r\033[K";
            show_packet(packet);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    recv_thread.join();
    std::cout << "session end" << std::endl;
}


