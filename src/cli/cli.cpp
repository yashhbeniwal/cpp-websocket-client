#include "cli.h"
#include <iostream>

CLI::CLI() : running_(true) {}

void CLI::set_input_callback(InputCallback callback) {
    input_callback_ = callback;
}

void CLI::print(const std::string& message) {
    std::cout << message << std::endl;
}

void CLI::run() {
    std::string input;

    std::cout << "Type messages to send. Type /quit to exit.\n";

    while (running_) {
        std::getline(std::cin, input);

        if (input == "/quit") {
            running_ = false;
            break;
        }

        if (input_callback_) {
            input_callback_(input);
        }
    }
}
