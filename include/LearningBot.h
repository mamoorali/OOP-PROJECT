#pragma once
#include "AbstractChatbot.h"
#include "LearningEngine.h"
#include <vector>

// A bot that actively learns new responses from the user during conversation
// When it doesn't know something, it asks the user what it should say
// Demonstrates: Inheritance + Composition (has-a LearningEngine)
class LearningBot : public AbstractChatbot {
private:
    LearningEngine* learner;

    bool        waitingForTeaching;   // true when bot asked "what should I say?"
    std::string pendingInput;         // the input we need a response for

    std::vector<std::string> curiosityPhrases;  // phrases for asking the user
    void initPhrases();

protected:
    std::string getPersonalityPrefix()                         const override;
    std::string applyPersonalityStyle(const std::string& raw) const override;

public:
    LearningBot(ResponseEngine* engine, LearningEngine* learner);

    std::string generateResponse(const std::string& input) override;
    std::string getStatusLine()                            const override;

    bool isWaitingForTeaching()  const;
    void cancelTeachingMode();
};
