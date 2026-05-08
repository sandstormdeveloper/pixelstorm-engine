#include "pixelstorm/core/Application.h"
#include "pixelstorm/core/Log.h"
#include "pixelstorm/core/Time.h"
#include "pixelstorm/core/Window.h"
#include "pixelstorm/renderer/Camera2D.h"
#include "pixelstorm/renderer/Renderer.h"
#include "pixelstorm/renderer/Shader.h"
#include "pixelstorm/renderer/Texture.h"
#include "pixelstorm/systems/RenderSystem.h"
#include <pixelstorm/input/Input.h>

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <memory>

Application::Application(int width, int height, const char *title)
    : m_World(m_Registry)
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
    // Run message
    Log::Info("Application main loop started.");

    // Loops until window is closed
    while (!m_Window->ShouldClose())
    {
        // Updates clock every frame
        Time::Update();

        // If an update callback has been defined
        if (m_UpdateCallback)
        {
            m_UpdateCallback(Time::GetDeltaTime());
        }

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

            // Renders
            if (m_Renderer)
            {
                m_RenderSystem->Render(m_Registry, *m_Renderer, *shaderToUse, m_Texture.get());
            }
        }

        // Updates window
        m_Window->Update();
    }

    // Run end message
    Log::Info("Application main loop finished.");
}

void Application::Init(int width, int height, const char *title)
{
    // Starts logger
    Log::Init();

    // Creates window with specified parameters
    m_Window = std::make_unique<Window>(width, height, title);

    // Sets window used by input
    Input::SetWindow(m_Window->GetNativeWindow());

    // Starts clock
    Time::Init();

    // Init message
    Log::Info("Application initialized.");

    // Creates renderer
    m_Renderer = std::make_unique<Renderer>();
    m_RenderSystem = std::make_unique<RenderSystem>();
    Log::Info("Renderer and render system created.");

    // Creates camera
    m_Camera = std::make_unique<Camera2D>(
        0.0f,
        static_cast<float>(width),
        static_cast<float>(height),
        0.0f);
    Log::Info("Main 2D camera created.");

    // Creates texture
    m_Texture = std::make_unique<Texture>();

    // Sets default shaders (can be overwritten)
    m_DefaultShader = std::make_unique<Shader>("default");
    m_EntityShader.reset();
    Log::Info("Default rendering resources created.");
}

void Application::Shutdown()
{
    // Resets clock
    Time::Shutdown();

    // Shutdown message
    Log::Info("Application shutdown.");

    // Destroys objects
    m_Texture.reset();
    m_RenderSystem.reset();
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

void Application::OnUpdate(const std::function<void(float)> &callback)
{
    // Stores game update callback
    m_UpdateCallback = callback;
    Log::Info("Application update callback registered.");
}

World &Application::GetWorld()
{
    // Returns persistent world handle
    return m_World;
}

const World &Application::GetWorld() const
{
    // Returns persistent world handle in read-only mode
    return m_World;
}
