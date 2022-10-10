#ifndef CHAT_SERVER_HELPER_H
#define CHAT_SERVER_HELPER_H

class server_helper {
    int port_;
public:
    server_helper(int port);
    void run();
};

#endif //CHAT_SERVER_HELPER_H
