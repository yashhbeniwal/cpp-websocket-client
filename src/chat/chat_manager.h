#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include "../websocket/websocket_client.h"
#include "../cli/cli.h"

class ChatManager {
public:
    ChatManager();

    // Start chat application
    void start(const std::string& uri);

private:
    WebSocketClient websocket_;
    CLI cli_;

    void setup_callbacks();
};

#endif // CHAT_MANAGER_H
