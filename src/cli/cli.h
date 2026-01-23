#ifndef CLI_H
#define CLI_H

#include <string>
#include <functional>

class CLI {
public:
    using InputCallback = std::function<void(const std::string&)>;

    CLI();

    // Start reading user input
    void run();

    // Print message to terminal
    void print(const std::string& message);

    // Register callback when user enters input
    void set_input_callback(InputCallback callback);

private:
    InputCallback input_callback_;
    bool running_;
};

#endif // CLI_H
