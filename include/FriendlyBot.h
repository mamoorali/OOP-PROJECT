#pragma once
#include "AbstractChatbot.h"
#include <vector>

// Warm, enthusiastic, encouraging personality
// Demonstrates: Inheritance from AbstractChatbot, method overriding
class FriendlyBot : public AbstractChatbot {
private:
    std::vector<std::string> greetingPhrases;
    std::vector<std::string> farewellPhrases;
    std::vector<std::string> encouragements;

    void initPhrases();

protected:
    std::string getPersonalityPrefix()                         const override;
    std::string applyPersonalityStyle(const std::string& raw) const override;

public:
    explicit FriendlyBot(ResponseEngine* engine);

    std::string generateResponse(const std::string& input) override;
    std::string getStatusLine()                            const override;
};
