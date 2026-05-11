#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

class ResponseEngine
{
private:
    map<string, vector<string>> keywordMap;
    vector<string> fallbackResponses;

    void loadDefaultKeywords();
    string normalize(const string& input) const;
    bool inputContains(const string& input, const string& keyword) const;
    string pickRandom(const vector<string>& options) const;

public:
    ResponseEngine();

    string getResponse(const string& input) const;
    void addKeywordResponse(const string& keyword, const string& response);
    void addFallback(const string& response);
    string getFallback() const;
    bool isGreeting(const string& input) const;
    bool isFarewell(const string& input) const;
    bool isQuestion(const string& input) const;
    bool isInsult(const string& input) const;
};
