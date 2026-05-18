#include "FileStorage.h"
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

FileStorage::FileStorage(const string& dataDir) : dataDir(dataDir)
{
    string normalized = dataDir;
    if (!normalized.empty() && normalized.back() != '/' && normalized.back() != '\\')
    {
        normalized += "/";
    }

    this->dataDir = normalized;
    kvFile = this->dataDir + "memory.dat";
    historyFile = this->dataDir + "history.dat";

    ensureDirectoryExists();
    loadCache();
}

void FileStorage::ensureDirectoryExists() const
{
    fs::create_directories(dataDir);
}

void FileStorage::loadCache()
{
    cache.clear();

    ifstream in(kvFile);
    string line;

    while (getline(in, line))
    {
        size_t pos = line.find('=');
        if (pos == string::npos)
        {
            continue;
        }

        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        cache[key] = value;
    }
}

void FileStorage::persistCache()
{
    ofstream out(kvFile, ios::trunc);

    for (const auto& pair : cache)
    {
        out << pair.first << "=" << pair.second << "\n";
    }
}

void FileStorage::save(const string& key, const string& value)
{
    cache[key] = value;
    persistCache();
}

string FileStorage::load(const string& key)
{
    auto it = cache.find(key);
    if (it != cache.end())
    {
        return it->second;
    }
    return "";
}

bool FileStorage::exists(const string& key)
{
    return cache.find(key) != cache.end();
}

void FileStorage::remove(const string& key)
{
    cache.erase(key);
    persistCache();
}

void FileStorage::saveMessage(const Message& msg)
{
    ofstream out(historyFile, ios::app);
    out << msg.toString() << "\n";
}

vector<Message> FileStorage::loadHistory()
{
    vector<Message> messages;
    ifstream in(historyFile);
    string line;

    while (getline(in, line))
    {
        if (!line.empty())
        {
            messages.push_back(Message::fromString(line));
        }
    }

    return messages;
}

void FileStorage::clearHistory()
{
    ofstream out(historyFile, ios::trunc);
}
