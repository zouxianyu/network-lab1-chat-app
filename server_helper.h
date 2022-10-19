#ifndef CHAT_SERVER_HELPER_H
#define CHAT_SERVER_HELPER_H

#include <set>
#include <mutex>

class server_helper {

    int port_;
    std::set<SOCKET> clients_;
    std::mutex m_;
public:
    server_helper(int port);
    void run();
};

#endif //CHAT_SERVER_HELPER_H
