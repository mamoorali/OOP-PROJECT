#pragma once
#include <string>
#include <map>

using namespace std;

class User
{
private:
    string name;
    map<string, string> facts;

public:
    User();
    explicit User(const string& name);

    void setName(const string& name);
    string getName() const;
    bool isNamed() const;

    void rememberFact(const string& key, const string& value);
    string recallFact(const string& key) const;
    bool knowsFact(const string& key) const;

    map<string, string> getAllFacts() const;
    void clearFacts();
};
