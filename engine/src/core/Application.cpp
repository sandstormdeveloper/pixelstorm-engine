#include "pixelstorm/core/Application.h"
#include "pixelstorm/core/Log.h"
#include "pixelstorm/core/Time.h"
#include <glad/glad.h>
#include <glm/vec4.hpp>
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

        // Loops through entities in registry with a transform and sprite renderer
        for (Entity entity : m_Registry.GetEntitiesWith<Transform, SpriteRenderer>())
        {
            // Gets components
            Transform &transform = m_Registry.GetComponent<Transform>(entity);
            SpriteRenderer &sprite = m_Registry.GetComponent<SpriteRenderer>(entity);

            // Updates transform
            transform.Rotation = static_cast<float>(Time::GetElapsedTime() * 45.0);

            // Draws entity
            if (m_Renderer && shaderToUse && sprite.Visible && sprite.TextureResource)
            {
                sprite.TextureResource->Bind();
                shaderToUse->SetVec4("u_Color", sprite.Color);
                m_Renderer->DrawQuad(*shaderToUse, transform.GetMatrix());
            }
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

    // Creates test entity with registry
    m_DemoEntity = m_Registry.CreateEntity();
    m_DemoEntity2 = m_Registry.CreateEntity();

    // Adds transform component to test entity
    m_Registry.AddComponent<Transform>(
        m_DemoEntity,
        glm::vec2(-0.75f, 0.0f),
        glm::vec2(0.75f, 0.75f),
        0.0f);

    m_Registry.AddComponent<Transform>(
        m_DemoEntity2,
        glm::vec2(0.75f, 0.0f),
        glm::vec2(0.75f, 0.75f),
        0.0f);

    // Adds sprite renderer component to test entity
    m_Registry.AddComponent<SpriteRenderer>(
        m_DemoEntity,
        m_Texture.get(),
        glm::vec4(1.0f, 0.9f, 0.4f, 1.0f));

    m_Registry.AddComponent<SpriteRenderer>(
        m_DemoEntity2,
        m_Texture.get(),
        glm::vec4(0.4f, 0.8f, 1.0f, 1.0f));

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
