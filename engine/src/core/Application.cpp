#include "pixelstorm/core/Application.h"
#include <glad/glad.h>

Application::Application(int width, int height, const char* title)
{
    m_Window = new Window(width, height, title);
}

void Application::Run()
{
    while (!m_Window->ShouldClose())
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Window->Update();
    }
}