#pragma once
#include <string>
#include "pixelstorm/core/Window.h"
#include "pixelstorm/renderer/Shader.h"

class Application
{
public:
    Application(int width, int height, const char *title);
    ~Application();

    void Run();

    void SetDefaultShader(const std::string &name);

private:
    Window *m_Window;

    Shader *m_DefaultShader;
    Shader *m_EntityShader;
};