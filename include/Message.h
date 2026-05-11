#pragma once
#include <string>

using namespace std;

class Message
{
private:
    string content;
    string sender;
    string timestamp;

    string getCurrentTime() const;

public:
    Message();
    Message(const string& content, const string& sender);

    string getContent() const;
    string getSender() const;
    string getTimestamp() const;
    string toString() const;

    static Message fromString(const string& line);
};
