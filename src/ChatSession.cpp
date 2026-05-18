#include "ChatSession.h"
#include <ctime>
#include <iostream>

using namespace std;

ChatSession::ChatSession(User* user)
    : sessionId(generateSessionId()), user(user), startTime(getCurrentTime())
{
}

string ChatSession::generateSessionId() const
{
    return "session_" + to_string(time(nullptr));
}

string ChatSession::getCurrentTime() const
{
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return string(buffer);
}

void ChatSession::addMessage(const Message& msg)
{
    history.push_back(msg);
}

void ChatSession::clearHistory()
{
    history.clear();
}

void ChatSession::saveSession(Storage* storage) const
{
    if (storage == nullptr)
    {
        return;
    }

    storage->clearHistory();

    for (const Message& msg : history)
    {
        storage->saveMessage(msg);
    }
}

void ChatSession::loadSession(Storage* storage)
{
    if (storage == nullptr)
    {
        return;
    }

    history = storage->loadHistory();
}

vector<Message> ChatSession::getHistory() const
{
    return history;
}

string ChatSession::getSessionId() const
{
    return sessionId;
}

User* ChatSession::getUser() const
{
    return user;
}

int ChatSession::getMessageCount() const
{
    return static_cast<int>(history.size());
}

string ChatSession::getStartTime() const
{
    return startTime;
}

void ChatSession::printHistory(int last) const
{
    int start = static_cast<int>(history.size()) - last;
    if (start < 0)
    {
        start = 0;
    }

    cout << "\n--- Chat History ---\n";
    for (int i = start; i < static_cast<int>(history.size()); i++)
    {
        cout << "[" << history[i].getTimestamp() << "] "
             << history[i].getSender() << ": "
             << history[i].getContent() << "\n";
    }
    cout << "--------------------\n";
}
