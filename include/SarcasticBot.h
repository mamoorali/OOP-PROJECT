#pragma once
#include "AbstractChatbot.h"
#include <vector>

// Witty, dry, sarcastic personality — always a little too clever
// Demonstrates: Inheritance from AbstractChatbot, distinct personality behavior
class SarcasticBot : public AbstractChatbot {
private:
    std::vector<std::string> sarcasticPrefixes;
    std::vector<std::string> sarcasticSuffixes;

    void initPhrases();
    std::string pickRandom(const std::vector<std::string>& v) const;

protected:
    std::string getPersonalityPrefix()                         const override;
    std::string applyPersonalityStyle(const std::string& raw) const override;

public:
    explicit SarcasticBot(ResponseEngine* engine);

    std::string generateResponse(const std::string& input) override;
    std::string getStatusLine()                            const override;
};
