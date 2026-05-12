#pragma once
#include <string>
#include <map>
#include "Storage.h"

// Manages what the bot remembers about the user
// Uses Storage for persistent memory, and an in-RAM map for session memory
// Demonstrates: Composition (has-a Storage), Encapsulation
class MemoryManager {
private:
    Storage* storage;
    std::map<std::string, std::string> sessionMemory;  // lost when app closes
    std::string memPrefix = "mem_";                    // storage key prefix

    std::string makeKey(const std::string& key) const;

public:
    explicit MemoryManager(Storage* storage);

    // ── Persistent Memory (saved to file / database) ───────────
    void        remember(const std::string& key, const std::string& value);
    std::string recall(const std::string& key)   const;
    bool        knows(const std::string& key)    const;
    void        forget(const std::string& key);

    // ── Session Memory (in-RAM only for this run) ──────────────
    void        rememberSession(const std::string& key, const std::string& value);
    std::string recallSession(const std::string& key)   const;
    bool        knowsSession(const std::string& key)    const;

    // Auto-extract facts from natural language
    // e.g., "my name is Alex" → saves name=Alex
    // Returns true if something was extracted
    bool extractAndStore(const std::string& input);
};
