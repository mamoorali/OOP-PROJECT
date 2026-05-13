#include "Logger.h"
#include <iostream>
#include <ctime>

using namespace std;

Logger::Logger()
{
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLogFile(const string& path)
{
    logFilePath = path;
    if (logFile.is_open())
    {
        logFile.close();
    }
    logFile.open(path, ios::app);
}

void Logger::log(const string& message)
{
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);

    if (logFile.is_open())
    {
        logFile << buffer << " " << message << "\n";
    }
}

void Logger::logInfo(const string& message)
{
    log("[INFO]  " + message);
}

void Logger::logError(const string& message)
{
    log("[ERROR] " + message);
    cerr << "[ERROR] " << message << endl;
}

void Logger::logWarning(const string& message)
{
    log("[WARN]  " + message);
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}
