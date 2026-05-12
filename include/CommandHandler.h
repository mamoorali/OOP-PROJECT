#pragma once
#include <string>
#include <map>
#include <functional>

// Handles slash-commands like /time, /date, /help, /clear
// Demonstrates: Encapsulation, use of std::function (function pointers)
class CommandHandler {
private:
    // command name → handler function
    std::map<std::string, std::function<std::string()>> commands;

    // Built-in implementations
    std::string handleTime()   const;
    std::string handleDate()   const;
    std::string handleHelp()   const;
    std::string handleJoke()   const;

public:
    CommandHandler();

    // Returns true if input starts with '/'
    bool isCommand(const std::string& input) const;

    // Dispatch command and return the result string
    std::string handle(const std::string& input) const;

    // Register a custom command: registerCommand("/flip", []{ return "Heads!"; });
    void registerCommand(const std::string& cmd, std::function<std::string()> handler);

    // Returns formatted list of all commands for /help
    std::string getCommandList() const;
};
