#include "SarcasticBot.h"
#include <cstdlib>

using namespace std;

SarcasticBot::SarcasticBot(ResponseEngine* engine)
    : AbstractChatbot("Zara", "Sarcastic", engine)
{
    initPhrases();
}

void SarcasticBot::initPhrases()
{
    sarcasticPrefixes = {
        "Oh wow, ",
        "How thrilling, ",
        "Sure, "
    };

    sarcasticSuffixes = {
        " Brilliant.",
        " You're welcome, I guess.",
        ""
    };
}

string SarcasticBot::pickRandom(const vector<string>& v) const
{
    return v[rand() % v.size()];
}

string SarcasticBot::getPersonalityPrefix() const
{
    return pickRandom(sarcasticPrefixes);
}

string SarcasticBot::applyPersonalityStyle(const string& raw) const
{
    return getPersonalityPrefix() + raw + pickRandom(sarcasticSuffixes);
}

string SarcasticBot::generateResponse(const string& input)
{
    if (responseEngine->isGreeting(input))
    {
        return "Oh great, another human. Hello.";
    }

    if (responseEngine->isFarewell(input))
    {
        return "Finally. Goodbye.";
    }

    string raw = responseEngine->getResponse(input);
    if (raw.empty())
    {
        raw = "I have absolutely no idea.";
    }

    return applyMoodModifier(applyPersonalityStyle(raw));
}

string SarcasticBot::getStatusLine() const
{
    return "[Zara | Sarcastic | Mood: " + getMoodAsString() + "]";
}
