#pragma once
#include <string>
#include <map>

#include "User.h"
#include "ChatSession.h"
#include "AbstractChatbot.h"
#include "MemoryManager.h"
#include "LearningEngine.h"
#include "ResponseEngine.h"
#include "CommandHandler.h"
#include "Storage.h"
#include "Logger.h"

// The central orchestrator — wires all components together
// Processing pipeline: Command? → Memory extract → Learned response →
//                      Keyword response → Personality response → Fallback
// Demonstrates: Composition (has-a everything), Polymorphism (AbstractChatbot*)
class ChatEngine {
private:
    // ── Owned components ───────────────────────────────────────
    User*           user;
    ChatSession*    session;
    MemoryManager*  memory;
    LearningEngine* learner;
    ResponseEngine* responder;
    CommandHandler* cmdHandler;
    Storage*        storage;   // non-owning (passed in by caller)

    // ── Personality registry ───────────────────────────────────
    std::map<std::string, AbstractChatbot*> personalities;
    AbstractChatbot* currentBot;    // pointer to one of the entries above

    // ── Internal helpers ───────────────────────────────────────
    void        initPersonalities();
    void        printWelcome()                              const;
    std::string buildMemoryAwareResponse(const std::string& raw,
                                         const std::string& input) const;
    void        registerCustomCommands();

public:
    explicit ChatEngine(Storage* storage);
    ~ChatEngine();

    // Call once before start()
    void initialize(const std::string& userName = "");

    // The full response pipeline — called for every user message
    std::string processInput(const std::string& input);

    // Switch active personality: "friendly" | "professional" | "sarcastic" | "sad" | "learning"
    void switchPersonality(const std::string& type);

    // Change the active bot's mood
    void setBotMood(BotMood mood);

    std::string getCurrentPersonality() const;
    std::string getBotStatus()          const;

    void saveSession();

    // Starts the console chat loop (replaces GUI for now)
    void start();
};
