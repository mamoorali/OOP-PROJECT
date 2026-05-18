#pragma once
#include "Storage.h"

using namespace std;

struct sqlite3;

class DatabaseStorage : public Storage
{
private:
    sqlite3* db;
    string dbPath;

    void initializeDB();
    bool executeSQL(const string& sql);

public:
    explicit DatabaseStorage(const string& dbPath = "data/chatbot.db");
    ~DatabaseStorage();

    void save(const string& key, const string& value) override;
    string load(const string& key) override;
    bool exists(const string& key) override;
    void remove(const string& key) override;

    void saveMessage(const Message& msg) override;
    vector<Message> loadHistory() override;
    void clearHistory() override;
};
