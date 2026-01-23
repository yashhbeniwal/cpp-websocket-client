#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <string>
#include <functional>

// Forward declaration (we don’t expose library internals here)
class WebSocketClient {
public:
    // Callback type for received messages
    using MessageCallback = std::function<void(const std::string&)>;

    WebSocketClient();
    ~WebSocketClient();

    // Connect to WebSocket server
    bool connect(const std::string& uri);

    // Send a text message
    void send(const std::string& message);

    // Close the connection
    void close();

    // Register callback for incoming messages
    void set_message_callback(MessageCallback callback);

private:
    // Hide implementation details (clean design)
    class Impl;
    Impl* impl_;
};

#endif // WEBSOCKET_CLIENT_H
