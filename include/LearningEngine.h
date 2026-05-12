#pragma once
#include <string>
#include <map>
#include "Storage.h"

// Allows the bot to learn new input→response pairs at runtime
// Demonstrates: Encapsulation, Composition (has-a Storage)
class LearningEngine {
private:
    Storage* storage;
    std::map<std::string, std::string> learnedResponses;  // normalized input → response
    std::string storagePrefix = "learn_";

    std::string normalize(const std::string& input) const;  // lowercase + trim
    void        loadFromStorage();

public:
    explicit LearningEngine(Storage* storage);

    // Teach the bot: when someone says 'input', reply with 'response'
    void        learn(const std::string& input, const std::string& response);

    // Look up a learned response
    std::string getResponse(const std::string& input)  const;
    bool        hasResponse(const std::string& input)  const;

    // Remove a learned response
    void        unlearn(const std::string& input);

    // Persist all learned responses to storage
    void        saveAll();

    int         getLearningCount() const;
};
