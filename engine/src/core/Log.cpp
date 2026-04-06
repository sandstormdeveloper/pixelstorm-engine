#include "pixelstorm/core/Log.h"
#include <iostream>

// Initial logger state
bool Log::s_Initialized = false;

void Log::Init()
{
    // Initializes logger
    s_Initialized = true;
    Info("Logger initialized.");
}

void Log::Shutdown()
{
    // If active, shuts down the logger
    if (s_Initialized)
    {
        Info("Logger shutdown.");
    }

    s_Initialized = false;
}

void Log::Info(const std::string &message)
{
    // Informative message
    Write("INFO", message, false);
}

void Log::Warning(const std::string &message)
{
    // Non-fatal warning message
    Write("WARN", message, false);
}

void Log::Error(const std::string &message)
{
    // Important error
    Write("ERROR", message, true);
}

void Log::Write(const char *level, const std::string &message, bool useErrorStream)
{
    // Decides output stream and prints message
    std::ostream &stream = useErrorStream ? std::cerr : std::cout;
    stream << '[' << level << "] " << message << std::endl;
}
