#include "MemoryManager.h"
#include <sstream>

using namespace std;

static void toLowerInput(string& text)
{
    for (size_t i = 0; i < text.size(); i++)
    {
        text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
    }
}

static bool isAllDigits(const string& text)
{
    if (text.empty())
    {
        return false;
    }

    for (size_t i = 0; i < text.size(); i++)
    {
        if (!isdigit(static_cast<unsigned char>(text[i])))
        {
            return false;
        }
    }

    return true;
}

static string trimSpaces(const string& text)
{
    size_t start = text.find_first_not_of(" \t\r\n");
    size_t end = text.find_last_not_of(" \t\r\n");

    if (start == string::npos || end == string::npos)
    {
        return "";
    }

    return text.substr(start, end - start + 1);
}

static string removeEndingPunctuation(const string& text)
{
    string value = trimSpaces(text);

    while (!value.empty())
    {
        char last = value[value.size() - 1];
        if (last == '.' || last == '!' || last == '?' || last == ',')
        {
            value.erase(value.size() - 1, 1);
            value = trimSpaces(value);
        }
        else
        {
            break;
        }
    }

    return value;
}

MemoryManager::MemoryManager(Storage* storage)
    : storage(storage), memPrefix("mem_")
{
}

string MemoryManager::makeKey(const string& key) const
{
    return memPrefix + key;
}

void MemoryManager::remember(const string& key, const string& value)
{
    if (storage != nullptr)
    {
        storage->save(makeKey(key), value);
    }
}

string MemoryManager::recall(const string& key) const
{
    if (storage == nullptr)
    {
        return "";
    }

    return storage->load(makeKey(key));
}

bool MemoryManager::knows(const string& key) const
{
    if (storage == nullptr)
    {
        return false;
    }

    return storage->exists(makeKey(key));
}

void MemoryManager::forget(const string& key)
{
    if (storage != nullptr)
    {
        storage->remove(makeKey(key));
    }
}

void MemoryManager::rememberSession(const string& key, const string& value)
{
    sessionMemory[key] = value;
}

string MemoryManager::recallSession(const string& key) const
{
    auto it = sessionMemory.find(key);
    if (it != sessionMemory.end())
    {
        return it->second;
    }
    return "";
}

bool MemoryManager::knowsSession(const string& key) const
{
    return sessionMemory.find(key) != sessionMemory.end();
}

bool MemoryManager::extractAndStore(const string& input)
{
    string low = input;
    toLowerInput(low);

    auto saveTail = [&](const string& prefix, const string& key) -> bool
    {
        size_t pos = low.find(prefix);
        if (pos != 0)
        {
            return false;
        }

        string value = input.substr(prefix.size());
        value = removeEndingPunctuation(value);

        if (value.empty())
        {
            return false;
        }

        remember(key, value);
        return true;
    };

    if (saveTail("my name is ", "user_name")) return true;
    if (saveTail("call me ", "user_name")) return true;
    if (saveTail("i am called ", "user_name")) return true;
    if (saveTail("my full name is ", "user_name")) return true;
    if (saveTail("i like ", "user_likes")) return true;
    if (saveTail("i love ", "user_likes")) return true;
    if (saveTail("i enjoy ", "user_likes")) return true;
    if (saveTail("my hobby is ", "user_hobby")) return true;
    if (saveTail("my hobbies are ", "user_hobby")) return true;
    if (saveTail("my favorite color is ", "user_favorite_color")) return true;
    if (saveTail("my favourite color is ", "user_favorite_color")) return true;
    if (saveTail("my favorite food is ", "user_favorite_food")) return true;
    if (saveTail("my favourite food is ", "user_favorite_food")) return true;
    if (saveTail("i live in ", "user_location")) return true;
    if (saveTail("i am from ", "user_location")) return true;
    if (saveTail("i'm from ", "user_location")) return true;
    if (saveTail("i work as ", "user_job")) return true;
    if (saveTail("i am a ", "user_job")) return true;
    if (saveTail("i'm a ", "user_job")) return true;
    if (saveTail("i study at ", "user_study_place")) return true;
    if (saveTail("i study in ", "user_study_place")) return true;
    if (saveTail("my university is ", "user_study_place")) return true;
    if (saveTail("my college is ", "user_study_place")) return true;
    if (saveTail("my professor is ", "user_professor")) return true;
    if (saveTail("my teacher is ", "user_professor")) return true;
    if (saveTail("i dislike ", "user_dislikes")) return true;
    if (saveTail("i hate ", "user_dislikes")) return true;

    size_t agePos = low.find("i am ");
    if (agePos == 0)
    {
        string tail = low.substr(5);
        istringstream stream(tail);
        string first;
        string second;
        stream >> first >> second;

        if (isAllDigits(first))
        {
            remember("user_age", first);
            return true;
        }
    }

    size_t yearsOldPos = low.find(" years old");
    if (yearsOldPos != string::npos)
    {
        istringstream stream(low);
        string first;
        string second;
        string third;
        stream >> first >> second >> third;

        if ((first == "i" && second == "am" && isAllDigits(third)) ||
            (first == "i'm" && isAllDigits(second)))
        {
            remember("user_age", isAllDigits(third) ? third : second);
            return true;
        }
    }

    return false;
}
