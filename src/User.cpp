#include "User.h"

using namespace std;

User::User() : name("")
{
}

User::User(const string& name) : name(name)
{
}

void User::setName(const string& name)
{
    this->name = name;
}

string User::getName() const
{
    return name;
}

bool User::isNamed() const
{
    return !name.empty();
}

void User::rememberFact(const string& key, const string& value)
{
    facts[key] = value;
}

string User::recallFact(const string& key) const
{
    auto it = facts.find(key);
    if (it != facts.end())
    {
        return it->second;
    }
    return "";
}

bool User::knowsFact(const string& key) const
{
    return facts.find(key) != facts.end();
}

map<string, string> User::getAllFacts() const
{
    return facts;
}

void User::clearFacts()
{
    facts.clear();
}
