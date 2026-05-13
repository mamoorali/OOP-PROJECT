#include "LearningEngine.h"

using namespace std;

static void toLowerText(string& text)
{
    for (size_t i = 0; i < text.size(); i++)
    {
        text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
    }
}

LearningEngine::LearningEngine(Storage* storage)
    : storage(storage), storagePrefix("learn_")
{
    loadFromStorage();
}

string LearningEngine::normalize(const string& input) const
{
    string out = input;
    toLowerText(out);

    size_t start = out.find_first_not_of(" \t\r\n");
    size_t end = out.find_last_not_of(" \t\r\n");

    if (start == string::npos || end == string::npos)
    {
        return "";
    }

    return out.substr(start, end - start + 1);
}

void LearningEngine::loadFromStorage()
{
    if (storage == nullptr || !storage->exists(storagePrefix + "count"))
    {
        return;
    }

    string countString = storage->load(storagePrefix + "count");
    int count = 0;

    if (!countString.empty())
    {
        count = stoi(countString);
    }

    for (int i = 0; i < count; i++)
    {
        string key = storage->load(storagePrefix + "key_" + to_string(i));
        string value = storage->load(storagePrefix + "value_" + to_string(i));

        if (!key.empty())
        {
            learnedResponses[key] = value;
        }
    }
}

void LearningEngine::learn(const string& input, const string& response)
{
    learnedResponses[normalize(input)] = response;
}

string LearningEngine::getResponse(const string& input) const
{
    auto it = learnedResponses.find(normalize(input));
    if (it != learnedResponses.end())
    {
        return it->second;
    }
    return "";
}

bool LearningEngine::hasResponse(const string& input) const
{
    return learnedResponses.find(normalize(input)) != learnedResponses.end();
}

void LearningEngine::unlearn(const string& input)
{
    learnedResponses.erase(normalize(input));
}

void LearningEngine::saveAll()
{
    if (storage == nullptr)
    {
        return;
    }

    int index = 0;
    for (const auto& pair : learnedResponses)
    {
        storage->save(storagePrefix + "key_" + to_string(index), pair.first);
        storage->save(storagePrefix + "value_" + to_string(index), pair.second);
        index++;
    }

    storage->save(storagePrefix + "count", to_string(index));
}

int LearningEngine::getLearningCount() const
{
    return static_cast<int>(learnedResponses.size());
}
