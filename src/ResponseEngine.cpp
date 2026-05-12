#include "ResponseEngine.h"
#include <cstdlib>
#include <ctime>

using namespace std;

static void toLowerResponseText(string& text)
{
    for (size_t i = 0; i < text.size(); i++)
    {
        text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
    }
}

ResponseEngine::ResponseEngine()
{
    srand(static_cast<unsigned>(time(nullptr)));
    loadDefaultKeywords();
}

string ResponseEngine::normalize(const string& input) const
{
    string out = input;
    toLowerResponseText(out);
    return out;
}

bool ResponseEngine::inputContains(const string& input, const string& keyword) const
{
    return normalize(input).find(normalize(keyword)) != string::npos;
}

string ResponseEngine::pickRandom(const vector<string>& options) const
{
    if (options.empty())
    {
        return "";
    }
    return options[rand() % options.size()];
}

void ResponseEngine::loadDefaultKeywords()
{
    keywordMap["hello"] = {"Hey there! How can I help?", "Hello! Great to chat with you.", "Hi! What's on your mind?"};
    keywordMap["hi"] = keywordMap["hello"];
    keywordMap["hey"] = keywordMap["hello"];
    keywordMap["good morning"] = {"Good morning! Hope your day is off to a great start.", "Morning! Ready to have a great day?"};
    keywordMap["good evening"] = {"Good evening! How was your day?", "Evening! Winding down or just getting started?"};

    keywordMap["bye"] = {"Goodbye! It was great chatting with you.", "See you later! Take care.", "Bye! Have a wonderful day."};
    keywordMap["goodbye"] = keywordMap["bye"];
    keywordMap["see you"] = {"See you! Come back anytime.", "Catch you later!"};

    keywordMap["how are you"] = {"I'm doing great, thanks for asking! How about you?", "Feeling wonderful! What about you?", "All systems running smoothly! You?"};
    keywordMap["what is your name"] = {"I'm Aria, your adaptive AI companion!", "You can call me Aria. Nice to meet you!"};
    keywordMap["who are you"] = {"I'm Aria, an intelligent chatbot built with C++ and OOP.", "I'm your personal chatbot. I can chat, learn, and remember things."};
    keywordMap["what can you do"] = {"I can chat, answer questions, remember things, and switch personalities.", "I can help you talk through ideas and remember facts you share."};

    keywordMap["i am sad"] = {"I'm sorry to hear that. Want to talk about it?", "I'm here for you. What happened?"};
    keywordMap["i am happy"] = {"That's wonderful! Tell me more.", "Love to hear it!"};
    keywordMap["i am bored"] = {"Let's fix that. Ask me something interesting.", "We can change that. What do you want to talk about?"};

    keywordMap["tell me a joke"] = {
        "Why do programmers prefer dark mode? Because light attracts bugs!",
        "Why did the developer go broke? Because they used up all their cache!",
        "Why do Java developers wear glasses? Because they don't C#!"
    };
    keywordMap["joke"] = keywordMap["tell me a joke"];
    keywordMap["funny"] = {"Want to hear a joke? Just ask!", "I know a few. Say 'tell me a joke'."};

    keywordMap["c++"] = {"C++ is a powerful language.", "C++ gives you a lot of control."};
    keywordMap["programming"] = {"I love programming talk! What are you working on?", "Coding is fascinating. What language are you using?"};
    keywordMap["oop"] = {"Object-Oriented Programming is all about modeling behavior and data together."};

    keywordMap["thank you"] = {"You're welcome!", "Happy to help!", "Anytime!"};
    keywordMap["thanks"] = keywordMap["thank you"];

    fallbackResponses = {
        "Hmm, I'm not sure I understand. Could you rephrase?",
        "That's an interesting one. I don't have an answer yet.",
        "I'm still learning. I don't have a response for that.",
        "Could you say that differently?"
    };
}

string ResponseEngine::getResponse(const string& input) const
{
    string low = normalize(input);
    string bestMatch;
    int bestLength = 0;

    for (const auto& pair : keywordMap)
    {
        if (low.find(pair.first) != string::npos)
        {
            if (static_cast<int>(pair.first.size()) > bestLength)
            {
                bestLength = static_cast<int>(pair.first.size());
                bestMatch = pair.first;
            }
        }
    }

    if (!bestMatch.empty())
    {
        return pickRandom(keywordMap.at(bestMatch));
    }

    return "";
}

void ResponseEngine::addKeywordResponse(const string& keyword, const string& response)
{
    keywordMap[normalize(keyword)].push_back(response);
}

void ResponseEngine::addFallback(const string& response)
{
    fallbackResponses.push_back(response);
}

string ResponseEngine::getFallback() const
{
    return pickRandom(fallbackResponses);
}

bool ResponseEngine::isGreeting(const string& input) const
{
    return inputContains(input, "hello") || inputContains(input, "hi") || inputContains(input, "hey");
}

bool ResponseEngine::isFarewell(const string& input) const
{
    return inputContains(input, "bye") || inputContains(input, "goodbye") || inputContains(input, "see you");
}

bool ResponseEngine::isQuestion(const string& input) const
{
    string low = normalize(input);
    return input.find('?') != string::npos ||
           low.find("what") == 0 ||
           low.find("how") == 0 ||
           low.find("why") == 0 ||
           low.find("when") == 0 ||
           low.find("who") == 0 ||
           low.find("can") == 0;
}

bool ResponseEngine::isInsult(const string& input) const
{
    return inputContains(input, "stupid") ||
           inputContains(input, "idiot") ||
           inputContains(input, "dumb") ||
           inputContains(input, "hate you");
}
