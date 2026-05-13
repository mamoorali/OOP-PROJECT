#pragma once
#include "AbstractChatbot.h"
#include <vector>

// Formal, concise, business-like personality
// Demonstrates: Inheritance from AbstractChatbot, distinct method overriding
class ProfessionalBot : public AbstractChatbot {
private:
    std::vector<std::string> formalGreetings;
    std::vector<std::string> formalFarewells;

    void initPhrases();

protected:
    std::string getPersonalityPrefix()                         const override;
    std::string applyPersonalityStyle(const std::string& raw) const override;

public:
    explicit ProfessionalBot(ResponseEngine* engine);

    std::string generateResponse(const std::string& input) override;
    std::string getStatusLine()                            const override;
};
