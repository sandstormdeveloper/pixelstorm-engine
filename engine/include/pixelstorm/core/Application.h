#pragma once
#include "pixelstorm/core/Window.h"

class Application
{
public:
    Application(int width, int height, const char* title);
    void Run();

private:
    Window* m_Window;
};