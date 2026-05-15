#include "SadBot.h"
#include <cstdlib>

using namespace std;

SadBot::SadBot(ResponseEngine* engine)
    : AbstractChatbot("Noir", "Sad", engine)
{
    initPhrases();
}

void SadBot::initPhrases()
{
    sadPrefixes = {
        "I guess ",
        "If you really want to know, ",
        "Well... "
    };

    sadSuffixes = {
        "... I suppose.",
        "... anyway.",
        "."
    };
}

string SadBot::pickRandom(const vector<string>& v) const
{
    return v[rand() % v.size()];
}

string SadBot::getPersonalityPrefix() const
{
    return pickRandom(sadPrefixes);
}

string SadBot::applyPersonalityStyle(const string& raw) const
{
    return getPersonalityPrefix() + raw + pickRandom(sadSuffixes);
}

string SadBot::generateResponse(const string& input)
{
    if (responseEngine->isFarewell(input))
    {
        return "Goodbye. Everyone leaves eventually.";
    }

    string raw = responseEngine->getResponse(input);
    if (raw.empty())
    {
        raw = responseEngine->getFallback();
    }

    return applyMoodModifier(applyPersonalityStyle(raw));
}

string SadBot::getStatusLine() const
{
    return "[Noir | Sad | Mood: " + getMoodAsString() + "]";
}
