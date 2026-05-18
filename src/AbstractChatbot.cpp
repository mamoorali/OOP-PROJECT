#include "AbstractChatbot.h"

using namespace std;

AbstractChatbot::AbstractChatbot(const string& name, const string& personalityType, ResponseEngine* engine)
    : moodPadding(0), name(name), personalityType(personalityType), currentMood(BotMood::NEUTRAL), responseEngine(engine)
{
}

AbstractChatbot::~AbstractChatbot()
{
}

void AbstractChatbot::setMood(BotMood mood)
{
    currentMood = mood;
}

BotMood AbstractChatbot::getMood() const
{
    return currentMood;
}

string AbstractChatbot::getMoodAsString() const
{
    switch (currentMood)
    {
    case BotMood::HAPPY:
        return "Happy";
    case BotMood::SAD:
        return "Sad";
    case BotMood::EXCITED:
        return "Excited";
    case BotMood::BORED:
        return "Bored";
    default:
        return "Neutral";
    }
}

string AbstractChatbot::getName() const
{
    return name;
}

string AbstractChatbot::getPersonalityType() const
{
    return personalityType;
}

string AbstractChatbot::applyMoodModifier(const string& response) const
{
    switch (currentMood)
    {
    case BotMood::HAPPY:
        return "I'm feeling great. " + response + " This makes me smile!";
    case BotMood::SAD:
        return "I'm feeling a little down. " + response + " ...";
    case BotMood::EXCITED:
        return "This is exciting! " + response + " I can't wait!";
    case BotMood::BORED:
        return "Honestly, I'm kind of bored. " + response + " Try surprising me.";
    default:
        return response;
    }
}

string AbstractChatbot::getStatusLine() const
{
    return "[" + name + " | " + personalityType + " | Mood: " + getMoodAsString() + "]";
}
