#include "pixelstorm/core/Application.h"
#include <glad/glad.h>
#include <iostream>

Application::Application(int width, int height, const char *title)
{
    // Creates window
    m_Window = new Window(width, height, title);

    // Loads shaders
    m_EntityShader = nullptr;
    m_DefaultShader = new Shader("default");
}

Application::~Application()
{
    // Frees memory
    delete m_DefaultShader;
    delete m_EntityShader;
    delete m_Window;
}

void Application::SetDefaultShader(const std::string &name)
{
    // Deletes old shader (if it exists)
    if (m_DefaultShader)
        delete m_DefaultShader;

    // Sets new shader
    m_DefaultShader = new Shader(name);
}

void Application::Run()
{
    // Loops until window is closed
    while (!m_Window->ShouldClose())
    {
        // Clears screen with background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Decides shader to use
        Shader *shaderToUse = m_EntityShader ? m_EntityShader : m_DefaultShader;

        // Activates shader
        if (shaderToUse)
            shaderToUse->Use();

        // Updates window
        m_Window->Update();
    }
}