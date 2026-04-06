#pragma once
#include <string>

class Log
{
public:
    static void Init();     // Initializes logger system
    static void Shutdown(); // Shuts down logger system

    static void Info(const std::string &message);    // Informative message
    static void Warning(const std::string &message); // Non-fatal warning message
    static void Error(const std::string &message);   // Important error

private:
    static void Write(const char *level, const std::string &message, bool useErrorStream); // Prints log message (normal output or errors)

    static bool s_Initialized; // Logger state
};
