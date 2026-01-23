#include "chat/chat_manager.h"
#include <iostream>

int main() {
    std::string uri = "ws://127.0.0.1:9002";

    std::cout << "C++ WebSocket Client Starting...\n";

    ChatManager chat;
    chat.start(uri);

    return 0;
}
