#include "ChatEngine.h"
#include "FriendlyBot.h"
#include "ProfessionalBot.h"
#include "SarcasticBot.h"
#include "SadBot.h"
#include "LearningBot.h"
#include <iostream>

using namespace std;

static void toLowerChatText(string& text)
{
    for (size_t i = 0; i < text.size(); i++)
    {
        text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
    }
}

static bool startsWithText(const string& text, const string& prefix)
{
    if (text.size() < prefix.size())
    {
        return false;
    }

    for (size_t i = 0; i < prefix.size(); i++)
    {
        if (text[i] != prefix[i])
        {
            return false;
        }
    }

    return true;
}

ChatEngine::ChatEngine(Storage* storage)
    : user(nullptr), session(nullptr), memory(nullptr), learner(nullptr), responder(nullptr), cmdHandler(nullptr), storage(storage), currentBot(nullptr)
{
}

ChatEngine::~ChatEngine()
{
    saveSession();

    delete user;
    delete session;
    delete memory;
    delete learner;
    delete responder;
    delete cmdHandler;

    for (auto& pair : personalities)
    {
        delete pair.second;
    }
}

void ChatEngine::initialize(const string& userName)
{
    Logger::getInstance().setLogFile("data/chatbot.log");
    Logger::getInstance().logInfo("ChatEngine initializing...");

    responder = new ResponseEngine();
    learner = new LearningEngine(storage);
    memory = new MemoryManager(storage);
    cmdHandler = new CommandHandler();

    user = new User();
    if (!userName.empty())
    {
        user->setName(userName);
    }
    else
    {
        string savedName = memory->recall("user_name");
        if (!savedName.empty())
        {
            user->setName(savedName);
        }
    }

    session = new ChatSession(user);
    session->loadSession(storage);

    initPersonalities();
    registerCustomCommands();

    Logger::getInstance().logInfo("ChatEngine ready.");
}

void ChatEngine::initPersonalities()
{
    personalities["friendly"] = new FriendlyBot(responder);
    personalities["professional"] = new ProfessionalBot(responder);
    personalities["sarcastic"] = new SarcasticBot(responder);
    personalities["sad"] = new SadBot(responder);
    personalities["learning"] = new LearningBot(responder, learner);
    currentBot = personalities["friendly"];
}

void ChatEngine::registerCustomCommands()
{
    cmdHandler->registerCommand("/memory", [this]() -> string
    {
        string out = "Here's what I remember about you:\n";
        string name = memory->recall("user_name");
        string age = memory->recall("user_age");
        string likes = memory->recall("user_likes");
        string dislikes = memory->recall("user_dislikes");
        string location = memory->recall("user_location");
        string job = memory->recall("user_job");
        string hobby = memory->recall("user_hobby");
        string favoriteColor = memory->recall("user_favorite_color");
        string favoriteFood = memory->recall("user_favorite_food");
        string studyPlace = memory->recall("user_study_place");
        string professor = memory->recall("user_professor");

        if (!name.empty()) out += "  Name: " + name + "\n";
        if (!age.empty()) out += "  Age: " + age + "\n";
        if (!likes.empty()) out += "  Likes: " + likes + "\n";
        if (!dislikes.empty()) out += "  Dislikes: " + dislikes + "\n";
        if (!location.empty()) out += "  Location: " + location + "\n";
        if (!job.empty()) out += "  Job: " + job + "\n";
        if (!hobby.empty()) out += "  Hobby: " + hobby + "\n";
        if (!favoriteColor.empty()) out += "  Favorite Color: " + favoriteColor + "\n";
        if (!favoriteFood.empty()) out += "  Favorite Food: " + favoriteFood + "\n";
        if (!studyPlace.empty()) out += "  Study Place: " + studyPlace + "\n";
        if (!professor.empty()) out += "  Professor: " + professor + "\n";

        if (out == "Here's what I remember about you:\n")
        {
            out += "  Nothing yet.";
        }

        return out;
    });

    cmdHandler->registerCommand("/forget", [this]() -> string
    {
        memory->forget("user_name");
        memory->forget("user_age");
        memory->forget("user_likes");
        memory->forget("user_dislikes");
        memory->forget("user_location");
        memory->forget("user_job");
        memory->forget("user_hobby");
        memory->forget("user_favorite_color");
        memory->forget("user_favorite_food");
        memory->forget("user_study_place");
        memory->forget("user_professor");
        user->setName("");
        return "Done. I cleared what I knew about you.";
    });

    cmdHandler->registerCommand("/history", [this]() -> string
    {
        session->printHistory(10);
        return "";
    });

    cmdHandler->registerCommand("/clear", [this]() -> string
    {
        session->clearHistory();
        storage->clearHistory();
        return "Chat history cleared.";
    });
}

string ChatEngine::getMemoryAnswer(const string& input) const
{
    string low = input;
    toLowerChatText(low);

    auto answerFor = [&](const string& key, const string& label, const string& emptyText) -> string
    {
        string value = memory->recall(key);
        if (value.empty())
        {
            return styleMemoryAnswer("", emptyText);
        }
        return styleMemoryAnswer("Your " + label + " is " + value + ".", emptyText);
    };

    if (startsWithText(low, "what is my name") || startsWithText(low, "do you know my name"))
    {
        return answerFor("user_name", "name", "You have not told me your name yet.");
    }

    if (startsWithText(low, "how old am i") || startsWithText(low, "what is my age"))
    {
        return answerFor("user_age", "age", "You have not told me your age yet.");
    }

    if (startsWithText(low, "what do i like") || startsWithText(low, "what are my likes"))
    {
        return answerFor("user_likes", "likes", "You have not told me what you like yet.");
    }

    if (startsWithText(low, "what do i dislike") || startsWithText(low, "what are my dislikes"))
    {
        return answerFor("user_dislikes", "dislikes", "You have not told me what you dislike yet.");
    }

    if (startsWithText(low, "where do i live") || startsWithText(low, "where am i from"))
    {
        return answerFor("user_location", "location", "You have not told me where you live yet.");
    }

    if (startsWithText(low, "what is my job") || startsWithText(low, "what do i do"))
    {
        return answerFor("user_job", "job", "You have not told me your job yet.");
    }

    if (startsWithText(low, "what is my hobby") || startsWithText(low, "what are my hobbies"))
    {
        return answerFor("user_hobby", "hobby", "You have not told me your hobby yet.");
    }

    if (startsWithText(low, "what is my favorite color") || startsWithText(low, "what is my favourite color"))
    {
        return answerFor("user_favorite_color", "favorite color", "You have not told me your favorite color yet.");
    }

    if (startsWithText(low, "what is my favorite food") || startsWithText(low, "what is my favourite food"))
    {
        return answerFor("user_favorite_food", "favorite food", "You have not told me your favorite food yet.");
    }

    if (startsWithText(low, "where do i study") || startsWithText(low, "what is my university") || startsWithText(low, "what is my college"))
    {
        return answerFor("user_study_place", "study place", "You have not told me where you study yet.");
    }

    if (startsWithText(low, "who is my professor") || startsWithText(low, "who is my teacher"))
    {
        string value = memory->recall("user_professor");
        if (value.empty())
        {
            return styleMemoryAnswer("", "You have not told me your professor yet.");
        }
        return styleMemoryAnswer("Your professor is " + value + ".", "You have not told me your professor yet.");
    }

    return "";
}

string ChatEngine::styleMemoryAnswer(const string& neutralAnswer, const string& missingAnswer) const
{
    string personality = getCurrentPersonality();
    string lowPersonality = personality;
    toLowerChatText(lowPersonality);

    if (neutralAnswer.empty())
    {
        if (lowPersonality == "friendly")
        {
            return missingAnswer + " Tell me and I'll remember it for you!";
        }
        if (lowPersonality == "professional")
        {
            return missingAnswer;
        }
        if (lowPersonality == "sarcastic")
        {
            return missingAnswer + " Amazing. You never told me.";
        }
        if (lowPersonality == "sad")
        {
            return missingAnswer + " I suppose I was not told.";
        }
        if (lowPersonality == "learning")
        {
            return missingAnswer + " You can teach me by telling me directly.";
        }
        return missingAnswer;
    }

    if (lowPersonality == "friendly")
    {
        return currentBot->getMood() == BotMood::NEUTRAL
            ? neutralAnswer + " I remembered that for you!"
            : currentBot->getMood() == BotMood::HAPPY
                ? neutralAnswer + " I remembered that for you! That makes me happy!"
                : currentBot->getMood() == BotMood::EXCITED
                    ? neutralAnswer + " I remembered that for you! Pretty exciting, right?"
                    : currentBot->getMood() == BotMood::SAD
                        ? neutralAnswer + " I remembered that for you, even if I feel a little down."
                        : neutralAnswer + " I remembered that for you, even though I'm a bit bored.";
    }
    if (lowPersonality == "professional")
    {
        return currentBot->getMood() == BotMood::NEUTRAL
            ? neutralAnswer
            : currentBot->getMood() == BotMood::HAPPY
                ? "Certainly. " + neutralAnswer
                : currentBot->getMood() == BotMood::EXCITED
                    ? "Certainly. " + neutralAnswer + " I am pleased to report that."
                    : currentBot->getMood() == BotMood::SAD
                        ? neutralAnswer + " I hope that is still useful."
                        : neutralAnswer + " That is the information I have.";
    }
    if (lowPersonality == "sarcastic")
    {
        return currentBot->getMood() == BotMood::NEUTRAL
            ? neutralAnswer + " You're welcome for my incredible memory."
            : currentBot->getMood() == BotMood::HAPPY
                ? neutralAnswer + " You're welcome. I am clearly brilliant today."
                : currentBot->getMood() == BotMood::EXCITED
                    ? neutralAnswer + " Can you believe I remembered that? Stunning."
                    : currentBot->getMood() == BotMood::SAD
                        ? neutralAnswer + " At least my memory still works."
                        : neutralAnswer + " There. A fact. Try to contain your excitement.";
    }
    if (lowPersonality == "sad")
    {
        return currentBot->getMood() == BotMood::NEUTRAL
            ? neutralAnswer + " At least I remembered that."
            : currentBot->getMood() == BotMood::HAPPY
                ? neutralAnswer + " I actually feel a little better remembering that."
                : currentBot->getMood() == BotMood::EXCITED
                    ? neutralAnswer + " I suppose that is something to be excited about."
                    : currentBot->getMood() == BotMood::SAD
                        ? neutralAnswer + " At least I remembered that."
                        : neutralAnswer + " I remembered it, somehow.";
    }
    if (lowPersonality == "learning")
    {
        return currentBot->getMood() == BotMood::NEUTRAL
            ? neutralAnswer + " I learned that from our earlier conversation."
            : currentBot->getMood() == BotMood::HAPPY
                ? neutralAnswer + " I learned that earlier, and I'm glad I did!"
                : currentBot->getMood() == BotMood::EXCITED
                    ? neutralAnswer + " I learned that earlier, and it is exciting to use it now!"
                    : currentBot->getMood() == BotMood::SAD
                        ? neutralAnswer + " I learned that earlier, even if I sound a little low."
                        : neutralAnswer + " I learned that earlier. Still useful, I guess.";
    }

    return neutralAnswer;
}

string ChatEngine::processInput(const string& input)
{
    if (input.empty())
    {
        return "";
    }

    Logger::getInstance().logInfo("User: " + input);
    session->addMessage(Message(input, "user"));
    storage->saveMessage(Message(input, "user"));

    if (cmdHandler->isCommand(input))
    {
        string low = input;
        toLowerChatText(low);

        if (low.find("/switch") == 0)
        {
            string type = low.size() > 8 ? low.substr(8) : "";
            size_t start = type.find_first_not_of(' ');
            if (start != string::npos)
            {
                type = type.substr(start);
            }

            switchPersonality(type);
            string response = "Switched to " + type + " personality!";
            session->addMessage(Message(response, "bot"));
            storage->saveMessage(Message(response, "bot"));
            return response;
        }

        if (low.find("/mood") == 0)
        {
            string moodString = low.size() > 6 ? low.substr(6) : "";
            size_t start = moodString.find_first_not_of(' ');
            if (start != string::npos)
            {
                moodString = moodString.substr(start);
            }

            BotMood mood = BotMood::NEUTRAL;
            if (moodString == "happy") mood = BotMood::HAPPY;
            else if (moodString == "sad") mood = BotMood::SAD;
            else if (moodString == "excited") mood = BotMood::EXCITED;
            else if (moodString == "bored") mood = BotMood::BORED;

            setBotMood(mood);

            string response = "Mood changed to " + currentBot->getMoodAsString() + ".";
            session->addMessage(Message(response, "bot"));
            storage->saveMessage(Message(response, "bot"));
            return response;
        }

        string response = cmdHandler->handle(input);
        if (!response.empty())
        {
            session->addMessage(Message(response, "bot"));
            storage->saveMessage(Message(response, "bot"));
        }
        return response;
    }

    bool learnedFact = memory->extractAndStore(input);
    string savedName = memory->recall("user_name");

    if (!savedName.empty() && !user->isNamed())
    {
        user->setName(savedName);
    }

    string response;

    if (learner->hasResponse(input))
    {
        response = learner->getResponse(input) + " [learned]";
    }

    if (response.empty() && !user->getName().empty() && learnedFact)
    {
        response = "Got it, " + user->getName() + "! I'll remember that.";
    }

    if (response.empty())
    {
        response = getMemoryAnswer(input);
    }

    if (response.empty())
    {
        response = currentBot->generateResponse(input);
    }

    session->addMessage(Message(response, "bot"));
    storage->saveMessage(Message(response, "bot"));
    Logger::getInstance().logInfo("Bot: " + response);

    return response;
}

void ChatEngine::switchPersonality(const string& type)
{
    auto it = personalities.find(type);
    if (it != personalities.end())
    {
        currentBot = it->second;
    }
    else
    {
        Logger::getInstance().logWarning("Unknown personality: " + type);
    }
}

void ChatEngine::setBotMood(BotMood mood)
{
    if (currentBot != nullptr)
    {
        currentBot->setMood(mood);
    }
}

string ChatEngine::getCurrentPersonality() const
{
    if (currentBot == nullptr)
    {
        return "None";
    }
    return currentBot->getPersonalityType();
}

string ChatEngine::getBotStatus() const
{
    if (currentBot == nullptr)
    {
        return "No active bot.";
    }
    return currentBot->getStatusLine();
}

void ChatEngine::saveSession()
{
    if (session != nullptr && storage != nullptr)
    {
        session->saveSession(storage);
    }

    if (learner != nullptr)
    {
        learner->saveAll();
    }
}

void ChatEngine::printWelcome() const
{
    cout << "\n";
    cout << "===========================================\n";
    cout << "   Modular Conversational AI Framework\n";
    cout << "===========================================\n";
    cout << getBotStatus() << "\n";
    cout << "Type /help for commands. Type 'bye' to quit.\n";
    cout << "===========================================\n\n";
}

void ChatEngine::start()
{
    initialize("");

    string storedName = memory->recall("user_name");

    if (storedName.empty())
    {
        cout << "Bot: Hi there! What's your name?\nYou: ";
        string name;
        getline(cin, name);

        if (!name.empty())
        {
            user->setName(name);
            memory->remember("user_name", name);
        }
    }
    else
    {
        user->setName(storedName);
        cout << "Bot: Welcome back, " << storedName << "!\n";
    }

    printWelcome();

    string input;

    while (true)
    {
        cout << "You: ";
        getline(cin, input);

        if (!cin)
        {
            break;
        }

        if (input.empty())
        {
            continue;
        }

        string low = input;
        toLowerChatText(low);

        if (low == "bye" || low == "exit" || low == "quit")
        {
            string farewell = processInput(input);
            cout << currentBot->getName() << ": " << farewell << "\n";
            break;
        }

        string response = processInput(input);
        if (!response.empty())
        {
            cout << currentBot->getName() << ": " << response << "\n\n";
        }
    }

    cout << "\n[Session ended. Goodbye!]\n";
    saveSession();
}
