#pragma once

#include "pixelstorm/core/Color.h"
#include "pixelstorm/core/Math.h"

#include <string>

class Application;

class UI
{
public:
    static void Bind(Application &app);                                                                                                                   // Binds the UI helper to the running application
    static void Unbind();                                                                                                                                 // Clears the bound application
    static void Print(const std::string &text, const Vec2 &position, const Color &color = Colors::White(), float scale = 1.0f, bool followCamera = true); // Queues text to be drawn by the application

private:
    static Application *s_App; // Application used to submit text drawing commands
};
