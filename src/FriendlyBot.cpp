#include "FriendlyBot.h"
#include <cstdlib>

using namespace std;

FriendlyBot::FriendlyBot(ResponseEngine* engine)
    : AbstractChatbot("Aria", "Friendly", engine)
{
    initPhrases();
}

void FriendlyBot::initPhrases()
{
    greetingPhrases = {
        "Hey there! So great to see you!",
        "Hi! You made my day by stopping by!",
        "Welcome back, friend!"
    };

    farewellPhrases = {
        "Aww, already leaving? Take care!",
        "Bye! You're awesome, remember that!",
        "See you soon!"
    };

    encouragements = {
        " You're doing amazing!",
        " Keep it up, you've got this!",
        " I believe in you!",
        ""
    };
}

string FriendlyBot::getPersonalityPrefix() const
{
    return "";
}

string FriendlyBot::applyPersonalityStyle(const string& raw) const
{
    string result = raw;

    if (rand() % 4 == 0)
    {
        result += encouragements[rand() % encouragements.size()];
    }

    return result;
}

string FriendlyBot::generateResponse(const string& input)
{
    if (responseEngine->isGreeting(input))
    {
        return greetingPhrases[rand() % greetingPhrases.size()];
    }

    if (responseEngine->isFarewell(input))
    {
        return farewellPhrases[rand() % farewellPhrases.size()];
    }

    string raw = responseEngine->getResponse(input);
    if (raw.empty())
    {
        raw = responseEngine->getFallback();
    }

    return applyMoodModifier(applyPersonalityStyle(raw));
}

string FriendlyBot::getStatusLine() const
{
    return "[Aria | Friendly | Mood: " + getMoodAsString() + "]";
}
