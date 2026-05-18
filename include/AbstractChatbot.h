#pragma once
#include <string>
#include "ResponseEngine.h"

using namespace std;

enum class BotMood
{
    NEUTRAL,
    HAPPY,
    SAD,
    EXCITED,
    BORED
};

class AbstractChatbot
{
protected:
    int moodPadding;
    string name;
    string personalityType;
    BotMood currentMood;
    ResponseEngine* responseEngine;

    virtual string getPersonalityPrefix() const = 0;
    virtual string applyPersonalityStyle(const string& raw) const = 0;
    string applyMoodModifier(const string& response) const;

public:
    AbstractChatbot(const string& name, const string& personalityType, ResponseEngine* engine);
    virtual ~AbstractChatbot();

    virtual string generateResponse(const string& input) = 0;

    void setMood(BotMood mood);
    BotMood getMood() const;
    string getMoodAsString() const;
    string getName() const;
    string getPersonalityType() const;
    virtual string getStatusLine() const;
};
