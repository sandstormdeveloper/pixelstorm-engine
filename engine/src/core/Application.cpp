#include "pixelstorm/core/Application.h"
#include <glad/glad.h>
#include <iostream>

Application::Application(int width, int height, const char* title)
{
    m_Window = new Window(width, height, title);

    m_EntityShader = nullptr;

    m_DefaultShader = new Shader("default");
}

Application::~Application()
{
    delete m_DefaultShader;
    delete m_EntityShader;
    delete m_Window;
}

void Application::SetDefaultShader(const std::string& name)
{
    if (m_DefaultShader)
        delete m_DefaultShader;

    m_DefaultShader = new Shader(name);
}

void Application::Run()
{
    while (!m_Window->ShouldClose())
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Shader* shaderToUse = m_EntityShader ? m_EntityShader : m_DefaultShader;

        if (shaderToUse)
            shaderToUse->Use();

        m_Window->Update();
    }
}