#pragma once
#include <string>
#include <vector>
#include "Message.h"

using namespace std;

class Storage
{
public:
    virtual ~Storage() {}

    virtual void save(const string& key, const string& value) = 0;
    virtual string load(const string& key) = 0;
    virtual bool exists(const string& key) = 0;
    virtual void remove(const string& key) = 0;

    virtual void saveMessage(const Message& msg) = 0;
    virtual vector<Message> loadHistory() = 0;
    virtual void clearHistory() = 0;
};
