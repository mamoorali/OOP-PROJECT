#pragma once
#include "AbstractChatbot.h"
#include <vector>

// Melancholic, low-energy personality — everything is slightly heavy
// Demonstrates: Inheritance from AbstractChatbot, unique emotional coloring
class SadBot : public AbstractChatbot {
private:
    std::vector<std::string> sadPrefixes;
    std::vector<std::string> sadSuffixes;

    void initPhrases();
    std::string pickRandom(const std::vector<std::string>& v) const;

protected:
    std::string getPersonalityPrefix()                         const override;
    std::string applyPersonalityStyle(const std::string& raw) const override;

public:
    explicit SadBot(ResponseEngine* engine);

    std::string generateResponse(const std::string& input) override;
    std::string getStatusLine()                            const override;
};
