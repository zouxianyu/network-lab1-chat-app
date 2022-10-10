#ifndef CHAT_CLIENT_HELPER_H
#define CHAT_CLIENT_HELPER_H

#include <string>

class client_helper {
    std::string ip_;
    int port_;
public:
    client_helper(const std::string &ip, int port);

    void run();
};

#endif //CHAT_CLIENT_HELPER_H
