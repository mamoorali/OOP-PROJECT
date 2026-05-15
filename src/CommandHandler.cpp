#include "CommandHandler.h"
#include <ctime>

using namespace std;

static void toLowerString(string& text)
{
    for (size_t i = 0; i < text.size(); i++)
    {
        text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
    }
}

CommandHandler::CommandHandler()
{
    commands["/time"] = [this]() { return handleTime(); };
    commands["/date"] = [this]() { return handleDate(); };
    commands["/help"] = [this]() { return handleHelp(); };
    commands["/joke"] = [this]() { return handleJoke(); };
}

bool CommandHandler::isCommand(const string& input) const
{
    return !input.empty() && input[0] == '/';
}

string CommandHandler::handle(const string& input) const
{
    string command = input;
    size_t space = input.find(' ');

    if (space != string::npos)
    {
        command = input.substr(0, space);
    }

    toLowerString(command);

    auto it = commands.find(command);
    if (it != commands.end())
    {
        return it->second();
    }

    return "Unknown command: " + command + ". Type /help to see available commands.";
}

void CommandHandler::registerCommand(const string& cmd, function<string()> handler)
{
    commands[cmd] = handler;
}

string CommandHandler::getCommandList() const
{
    string list = "Available Commands:\n";
    list += "  /time    - Show current time\n";
    list += "  /date    - Show today's date\n";
    list += "  /help    - Show this help menu\n";
    list += "  /joke    - Tell a random joke\n";
    list += "  /clear   - Clear chat history\n";
    list += "  /switch [friendly|professional|sarcastic|sad|learning] - Change personality\n";
    list += "  /mood [happy|sad|excited|bored|neutral] - Change bot mood\n";
    list += "  /memory  - Show what the bot remembers about you\n";
    list += "  /forget  - Clear the bot's memory\n";
    list += "  /history - Show recent chat history\n";
    return list;
}

string CommandHandler::handleTime() const
{
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", t);
    return string("Current time: ") + buffer;
}

string CommandHandler::handleDate() const
{
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%A, %B %d %Y", t);
    return string("Today is: ") + buffer;
}

string CommandHandler::handleHelp() const
{
    return getCommandList();
}

string CommandHandler::handleJoke() const
{
    vector<string> jokes = {
        "Why do programmers prefer dark mode? Because light attracts bugs!",
        "Why did the developer go broke? Because they used up all their cache!",
        "Why do Java developers wear glasses? Because they don't C#!",
        "Why was the computer cold? It left its Windows open.",
        "Why did the laptop go to the doctor? It had a virus.",
        "Why did the student eat his homework? Because the teacher said it was a piece of cake.",
        "Why can't a computer take its hat off? Because it has CAPS LOCK on.",
        "Why did the programmer quit his job? Because he did not get arrays.",
        "What is a computer's favorite snack? Microchips.",
        "Why did the keyboard break up with the monitor? There was no connection.",
        "Why are programmers bad at nature? Too many bugs.",
        "Why was the math book sad? It had too many problems.",
        "Why did the computer keep sneezing? It had too many bytes in the dust.",
        "What do you call 8 hobbits? A hobbyte.",
        "Why was the phone wearing glasses? It lost its contacts.",
        "Why did the function return early? It had too many arguments.",
        "Why was the C++ book so confident? Because it had class.",
        "Why did the loop get tired? It kept going around in circles.",
        "Why did the coder bring a ladder? To reach the high level language.",
        "Why did the database break up with the app? Too many queries.",
        "Why did the bug go to school? To improve its code quality."
    };

    return jokes[rand() % jokes.size()];
}
