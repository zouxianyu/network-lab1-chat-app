#include <iostream>
#include <string>
#include <windows.h>
#include "client_helper.h"
#include "server_helper.h"

std::string g_name;

static void show_help_info() {
    std::cout << std::endl;
    std::cout << "==========main window==========" << std::endl;
    std::cout << "help : show help info" << std::endl;
    std::cout << "exit : exit the program" << std::endl;
    std::cout << "listen <port> : listen on a port" << std::endl;
    std::cout << "connect <ip> <port> : connect to a server" << std::endl;
    std::cout << "setname <new name>" << std::endl;
    std::cout << std::endl;
    std::cout << "==========chat window==========" << std::endl;
    std::cout << "!exit : end chat session" << std::endl;
    std::cout << std::endl;
}

static void listen_parser() {
    std::string port;
    std::cin >> port;
    server_helper helper(std::stoi(port));
    std::cout << "..." << std::endl;
    helper.run();
}

static void connect_parser() {
    std::string ip, port;
    std::cin >> ip >> port;
    client_helper helper(ip, std::stoi(port));
    std::cout << "..." << std::endl;
    helper.run();
}

static void setname_parser() {
    std::cin >> g_name;
}

int main() {
    // init winsock
    WSADATA wsaData{0};
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // init chat name
    srand(time(0));
    int randint = rand() % 1000;
    g_name = "meme" + std::to_string(randint);

    // show welcome info
    std::cout << "welcome to use the chat app" << std::endl;
    std::cout << "your default name : " << g_name << std::endl;
    std::cout << "enter 'help' to get more info" << std::endl;
    std::cout << std::endl;

    bool exit = false;
    while (!exit) {
        // get command
        std::string cmd;
        std::cin >> cmd;

        // parse command
        if (cmd == "help") {
            show_help_info();
        } else if (cmd == "connect") {
            connect_parser();
        } else if (cmd == "listen") {
            listen_parser();
        } else if (cmd == "setname") {
            setname_parser();
        } else if (cmd == "exit") {
            exit = true;
        } else {
            std::cout << "unknown command" << std::endl;
        }
    }

    // winsock cleanup
    WSACleanup();
}
