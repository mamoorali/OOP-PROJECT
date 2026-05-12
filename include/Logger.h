#pragma once
#include <string>
#include <fstream>

// Singleton Logger — only one instance exists across the whole program
// Demonstrates: Singleton design pattern, Encapsulation
class Logger {
private:
    std::string   logFilePath;
    std::ofstream logFile;

    // Private constructor: prevents creating Logger with 'new Logger()'
    Logger() = default;
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

public:
    // Global access point — call Logger::getInstance() anywhere
    static Logger& getInstance();

    void setLogFile(const std::string& path);

    void log(const std::string& message);        // General log
    void logInfo(const std::string& message);    // [INFO] tag
    void logError(const std::string& message);   // [ERROR] tag
    void logWarning(const std::string& message); // [WARN] tag

    ~Logger();
};
