#pragma once
#include <string>
#include <vector>
#include "Message.h"
#include "User.h"
#include "Storage.h"

using namespace std;

class ChatSession
{
private:
    string sessionId;
    User* user;
    vector<Message> history;
    string startTime;

    string generateSessionId() const;
    string getCurrentTime() const;

public:
    explicit ChatSession(User* user);

    void addMessage(const Message& msg);
    void clearHistory();
    void saveSession(Storage* storage) const;
    void loadSession(Storage* storage);

    vector<Message> getHistory() const;
    string getSessionId() const;
    User* getUser() const;
    int getMessageCount() const;
    string getStartTime() const;

    string getHistoryText(int last = 10) const;
    void printHistory(int last = 10) const;
};
