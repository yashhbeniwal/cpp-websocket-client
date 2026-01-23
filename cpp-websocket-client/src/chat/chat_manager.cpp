#include "chat_manager.h"
#include <iostream>

ChatManager::ChatManager() {}

void ChatManager::setup_callbacks() {
    // When user types a message → send via WebSocket
    cli_.set_input_callback(
        [this](const std::string& input) {
            websocket_.send(input);
        }
    );

    // When server sends message → print to CLI
    websocket_.set_message_callback(
        [this](const std::string& message) {
            cli_.print("Server: " + message);
        }
    );
}

void ChatManager::start(const std::string& uri) {
    std::cout << "Connecting to " << uri << "...\n";

    if (!websocket_.connect(uri)) {
        std::cerr << "Failed to connect to server\n";
        return;
    }

    std::cout << "Connected successfully!\n";

    setup_callbacks();

    cli_.run();

    websocket_.close();
    std::cout << "Disconnected. Goodbye!\n";
}
