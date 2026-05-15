#include "ProfessionalBot.h"
#include <cstdlib>

using namespace std;

ProfessionalBot::ProfessionalBot(ResponseEngine* engine)
    : AbstractChatbot("Atlas", "Professional", engine)
{
    initPhrases();
}

void ProfessionalBot::initPhrases()
{
    formalGreetings = {
        "Hello. How may I assist you today?",
        "Good day. What would you like to discuss?",
        "Welcome. How can I help?"
    };

    formalFarewells = {
        "Goodbye. Take care.",
        "It was a pleasure assisting you.",
        "Farewell."
    };
}

string ProfessionalBot::getPersonalityPrefix() const
{
    return "";
}

string ProfessionalBot::applyPersonalityStyle(const string& raw) const
{
    return raw;
}

string ProfessionalBot::generateResponse(const string& input)
{
    if (responseEngine->isGreeting(input))
    {
        return formalGreetings[rand() % formalGreetings.size()];
    }

    if (responseEngine->isFarewell(input))
    {
        return formalFarewells[rand() % formalFarewells.size()];
    }

    string raw = responseEngine->getResponse(input);
    if (raw.empty())
    {
        raw = responseEngine->getFallback();
    }

    return applyMoodModifier(applyPersonalityStyle(raw));
}

string ProfessionalBot::getStatusLine() const
{
    return "[Atlas | Professional | Mood: " + getMoodAsString() + "]";
}
