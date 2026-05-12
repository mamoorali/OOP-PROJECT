#include "Message.h"
#include <ctime>

using namespace std;

Message::Message() : content(""), sender(""), timestamp(getCurrentTime())
{
}

Message::Message(const string& content, const string& sender)
    : content(content), sender(sender), timestamp(getCurrentTime())
{
}

string Message::getCurrentTime() const
{
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return string(buffer);
}

string Message::getContent() const
{
    return content;
}

string Message::getSender() const
{
    return sender;
}

string Message::getTimestamp() const
{
    return timestamp;
}

string Message::toString() const
{
    return timestamp + "|" + sender + "|" + content;
}

Message Message::fromString(const string& line)
{
    size_t first = line.find('|');
    size_t second = line.find('|', first == string::npos ? string::npos : first + 1);

    if (first == string::npos || second == string::npos)
    {
        return Message(line, "unknown");
    }

    Message msg(line.substr(second + 1), line.substr(first + 1, second - first - 1));
    msg.timestamp = line.substr(0, first);
    return msg;
}
