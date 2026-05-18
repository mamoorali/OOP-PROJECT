#pragma once
#include "Storage.h"
#include <map>

using namespace std;

class FileStorage : public Storage
{
private:
    string dataDir;
    string kvFile;
    string historyFile;
    map<string, string> cache;

    void loadCache();
    void persistCache();
    void ensureDirectoryExists() const;

public:
    explicit FileStorage(const string& dataDir = "data/");

    void save(const string& key, const string& value) override;
    string load(const string& key) override;
    bool exists(const string& key) override;
    void remove(const string& key) override;

    void saveMessage(const Message& msg) override;
    vector<Message> loadHistory() override;
    void clearHistory() override;
};
