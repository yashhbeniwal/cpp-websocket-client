#include "websocket_client.h"

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <thread>

// Type aliases
using websocketpp::connection_hdl;
using ws_client = websocketpp::client<websocketpp::config::asio_client>;

class WebSocketClient::Impl {
public:
    ws_client client_;
    connection_hdl hdl_;
    MessageCallback message_callback_;
    std::thread network_thread_;
    bool connected_ = false;

    Impl() {
        // Initialize ASIO
        client_.init_asio();

        // Message handler
        client_.set_message_handler(
            [this](connection_hdl, ws_client::message_ptr msg) {
                if (message_callback_) {
                    message_callback_(msg->get_payload());
                }
            }
        );

        // Failure handler
        client_.set_fail_handler(
            [this](connection_hdl) {
                std::cerr << "Connection failed\n";
                connected_ = false;
            }
        );

        // Close handler
        client_.set_close_handler(
            [this](connection_hdl) {
                std::cerr << "Connection closed\n";
                connected_ = false;
            }
        );
    }
};

WebSocketClient::WebSocketClient()
    : impl_(new Impl()) {}

WebSocketClient::~WebSocketClient() {
    close();
    delete impl_;
}

bool WebSocketClient::connect(const std::string& uri) {
    websocketpp::lib::error_code ec;
    auto con = impl_->client_.get_connection(uri, ec);

    if (ec) {
        std::cerr << "Connection error: " << ec.message() << std::endl;
        return false;
    }

    impl_->hdl_ = con->get_handle();
    impl_->client_.connect(con);

    impl_->network_thread_ = std::thread([this]() {
        impl_->client_.run();
    });

    impl_->connected_ = true;
    return true;
}

void WebSocketClient::send(const std::string& message) {
    if (!impl_->connected_) {
        std::cerr << "Not connected\n";
        return;
    }

    websocketpp::lib::error_code ec;
    impl_->client_.send(
        impl_->hdl_,
        message,
        websocketpp::frame::opcode::text,
        ec
    );

    if (ec) {
        std::cerr << "Send failed: " << ec.message() << std::endl;
    }
}

void WebSocketClient::close() {
    if (!impl_->connected_) return;

    websocketpp::lib::error_code ec;
    impl_->client_.close(
        impl_->hdl_,
        websocketpp::close::status::normal,
        "Client closing",
        ec
    );

    if (impl_->network_thread_.joinable()) {
        impl_->network_thread_.join();
    }

    impl_->connected_ = false;
}

void WebSocketClient::set_message_callback(MessageCallback callback) {
    impl_->message_callback_ = callback;
}
