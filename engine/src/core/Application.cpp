#include "pixelstorm/core/Application.h"
#include "pixelstorm/core/Log.h"
#include "pixelstorm/core/Time.h"
#include <glad/glad.h>
#include <glm/vec2.hpp>
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

        // Activates shader (if it exists)
        if (shaderToUse)
        {
            shaderToUse->Use();

            // Defines what texture to use
            shaderToUse->SetInt("u_Texture", 0);

            // Sets camera matrix
            if (m_Camera)
            {
                shaderToUse->SetMat4("u_ViewProjection", m_Camera->GetViewProjectionMatrix());
            }
        }

        // Binds texture
        if (m_Texture)
        {
            m_Texture->Bind();
        }

        // Draws quad
        if (m_Renderer && shaderToUse)
        {
            m_Renderer->DrawQuad(*shaderToUse, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f);
        }

        // Updates window
        m_Window->Update();
    }
}

void Application::Init(int width, int height, const char *title)
{
    // Starts logger
    Log::Init();

    // Creates window with specified parameters
    m_Window = std::make_unique<Window>(width, height, title);

    // Starts clock
    Time::Init();

    // Init message
    Log::Info("Application initialized.");

    // Creates renderer
    m_Renderer = std::make_unique<Renderer>();

    // Creates camera
    const float safeHeight = height > 0 ? static_cast<float>(height) : 1.0f;
    const float aspectRatio = static_cast<float>(width) / safeHeight;
    m_Camera = std::make_unique<Camera2D>(-aspectRatio, aspectRatio, -1.0f, 1.0f);

    // Creates texture
    m_Texture = std::make_unique<Texture>();

    // Sets default shaders (can be overwritten)
    m_DefaultShader = std::make_unique<Shader>("default");
    m_EntityShader.reset();
}

void Application::Shutdown()
{
    // Resets clock
    Time::Shutdown();

    // Shutdown message
    Log::Info("Application shutdown.");

    // Destroys objects
    m_Texture.reset();
    m_Camera.reset();
    m_Renderer.reset();
    m_EntityShader.reset();
    m_DefaultShader.reset();
    m_Window.reset();

    // Shuts down logger
    Log::Shutdown();
}

Shader *Application::GetActiveShader() const
{
    // Only uses entity shader if not null
    return m_EntityShader ? m_EntityShader.get() : m_DefaultShader.get();
}
