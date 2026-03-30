#pragma once
#include <string>
#include "pixelstorm/core/Window.h"
#include "pixelstorm/renderer/Shader.h"

class Application
{
public:
    Application(int width, int height, const char *title);
    ~Application();

    void Run();                                     // Game loop
    void SetDefaultShader(const std::string &name); // Sets default shader used

private:
    Window *m_Window; // Pointer to window

    Shader *m_DefaultShader; // Base shader
    Shader *m_EntityShader;  // Shader for objects
};