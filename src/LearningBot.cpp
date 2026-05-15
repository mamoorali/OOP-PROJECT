#include "LearningBot.h"
#include <cstdlib>

using namespace std;

LearningBot::LearningBot(ResponseEngine* engine, LearningEngine* learner)
    : AbstractChatbot("Echo", "Learning", engine), learner(learner), waitingForTeaching(false)
{
    initPhrases();
}

void LearningBot::initPhrases()
{
    curiosityPhrases = {
        "I do not know how to answer that yet. What should I say?",
        "Teach me how to respond to that.",
        "I am still learning. What would be a good reply?"
    };
}

string LearningBot::getPersonalityPrefix() const
{
    return "";
}

string LearningBot::applyPersonalityStyle(const string& raw) const
{
    return raw;
}

string LearningBot::generateResponse(const string& input)
{
    if (waitingForTeaching)
    {
        learner->learn(pendingInput, input);
        waitingForTeaching = false;
        pendingInput = "";
        return "Thanks. I learned a new response.";
    }

    if (learner->hasResponse(input))
    {
        return applyMoodModifier(learner->getResponse(input));
    }

    string raw = responseEngine->getResponse(input);
    if (!raw.empty())
    {
        return applyMoodModifier(raw);
    }

    waitingForTeaching = true;
    pendingInput = input;
    return curiosityPhrases[rand() % curiosityPhrases.size()];
}

string LearningBot::getStatusLine() const
{
    return "[Echo | Learning | Learned: " + to_string(learner->getLearningCount()) + "]";
}

bool LearningBot::isWaitingForTeaching() const
{
    return waitingForTeaching;
}

void LearningBot::cancelTeachingMode()
{
    waitingForTeaching = false;
    pendingInput = "";
}
