#pragma once
#include <string>

class Application;
class Entity;
class Input;
class Registry;
class Shader;
class Texture;
class Window;
class World;

class Log
{
public:
    static void Debug(const std::string &message); // Debug message for gameplay or engine diagnostics

private:
    friend class Application;
    friend class Entity;
    friend class Input;
    friend class Registry;
    friend class Shader;
    friend class Texture;
    friend class Font;
    friend class Window;
    friend class World;

    static void Init();     // Initializes logger system
    static void Shutdown(); // Shuts down logger system

    static void Info(const std::string &message);    // Informative message
    static void Warning(const std::string &message); // Non-fatal warning message
    static void Error(const std::string &message);   // Important error

    static void Write(const char *level, const std::string &message, bool useErrorStream); // Prints log message (normal output or errors)

    static bool s_Initialized; // Logger state
};
