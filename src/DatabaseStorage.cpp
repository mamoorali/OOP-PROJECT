#include "DatabaseStorage.h"
#include "FileStorage.h"

using namespace std;

DatabaseStorage::DatabaseStorage(const string& dbPath) : db(nullptr), dbPath(dbPath)
{
    initializeDB();
}

DatabaseStorage::~DatabaseStorage()
{
}

void DatabaseStorage::initializeDB()
{
}

bool DatabaseStorage::executeSQL(const string& sql)
{
    (void)sql;
    return true;
}

void DatabaseStorage::save(const string& key, const string& value)
{
    FileStorage fallback("data/");
    fallback.save(key, value);
}

string DatabaseStorage::load(const string& key)
{
    FileStorage fallback("data/");
    return fallback.load(key);
}

bool DatabaseStorage::exists(const string& key)
{
    FileStorage fallback("data/");
    return fallback.exists(key);
}

void DatabaseStorage::remove(const string& key)
{
    FileStorage fallback("data/");
    fallback.remove(key);
}

void DatabaseStorage::saveMessage(const Message& msg)
{
    FileStorage fallback("data/");
    fallback.saveMessage(msg);
}

vector<Message> DatabaseStorage::loadHistory()
{
    FileStorage fallback("data/");
    return fallback.loadHistory();
}

void DatabaseStorage::clearHistory()
{
    FileStorage fallback("data/");
    fallback.clearHistory();
}
