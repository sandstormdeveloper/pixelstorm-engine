#include "pixelstorm/core/Application.h"
#include "pixelstorm/core/Time.h"
#include <glad/glad.h>
#include <memory>

Application::Application(int width, int height, const char *title)
{
    // Initializes the application
    Init(width, height, title);
}

Application::~Application()
{
    // Shuts down the application
    Shutdown();
}

void Application::SetDefaultShader(const std::string &name)
{
    // Sets the base/default shader
    m_DefaultShader = std::make_unique<Shader>(name);
}

void Application::Run()
{
    // Loops until window is closed
    while (!m_Window->ShouldClose())
    {
        // Updates clock every frame
        Time::Update();

        // Clears screen with background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Decides shader to use
        Shader *shaderToUse = GetActiveShader();

        // Activates shader
        if (shaderToUse)
            shaderToUse->Use();

        // Updates window
        m_Window->Update();
    }
}

void Application::Init(int width, int height, const char *title)
{
    // Creates window with specified parameters
    m_Window = std::make_unique<Window>(width, height, title);

    // Starts clock
    Time::Init();

    // Sets default shaders (can be overwritten)
    m_DefaultShader = std::make_unique<Shader>("default");
    m_EntityShader.reset();
}

void Application::Shutdown()
{
    // Resets clock
    Time::Shutdown();

    // Destroys objects
    m_EntityShader.reset();
    m_DefaultShader.reset();
    m_Window.reset();
}

Shader *Application::GetActiveShader() const
{
    // Only uses entity shader if not null
    return m_EntityShader ? m_EntityShader.get() : m_DefaultShader.get();
}
