#include "pixelstorm/core/Application.h"
#include "pixelstorm/core/Log.h"
#include "pixelstorm/core/Time.h"
#include "pixelstorm/core/Window.h"
#include "pixelstorm/renderer/Font.h"
#include "pixelstorm/renderer/Camera2D.h"
#include "pixelstorm/renderer/Renderer.h"
#include "pixelstorm/renderer/Shader.h"
#include "pixelstorm/renderer/Texture.h"
#include "pixelstorm/resources/ResourceManager.h"
#include "pixelstorm/systems/AnimationSystem.h"
#include "pixelstorm/systems/ParticleSystem.h"
#include "pixelstorm/systems/PhysicsSystem.h"
#include "pixelstorm/systems/RenderSystem.h"
#include <pixelstorm/input/Input.h>

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <cmath>
#include <memory>

namespace
{
    constexpr float FontOversampleFactor = 4.0f;

    std::string BuildFontKey(const std::string &name, float pixelHeight)
    {
        return name + "@" + std::to_string(static_cast<int>(std::lround(pixelHeight)));
    }
}

Application::Application(int width, int height, const char *title)
    : m_World(m_Registry),
      m_SceneManager(m_World),
      m_DebugDrawColliders(false),
      m_DefaultFontScale(1.0f)
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

bool Application::LoadTexture(const std::string &name, const std::string &assetPath)
{
    // Rejects invalid resource manager state
    if (!m_ResourceManager)
    {
        Log::Error("Cannot load texture without a valid resource manager.");
        return false;
    }

    // Loads texture by logical name
    const bool loaded = m_ResourceManager->LoadTexture(name, assetPath);
    if (loaded)
    {
        Log::Info("Texture registered: " + name + " <- " + assetPath);
    }
    else
    {
        Log::Warning("Texture could not be loaded: " + name + " <- " + assetPath);
    }

    return loaded;
}

bool Application::LoadFont(const std::string &name, const std::string &assetPath, float pixelHeight)
{
    // Rejects invalid resource manager state
    if (!m_ResourceManager)
    {
        Log::Error("Cannot load font without a valid resource manager.");
        return false;
    }

    // Loads font by logical name
    const bool loaded = m_ResourceManager->LoadFont(name, assetPath, pixelHeight);
    if (loaded)
    {
        if (m_DefaultFontName.empty())
        {
            m_DefaultFontName = name;
        }

        Log::Info("Font registered: " + name + " <- " + assetPath);
    }
    else
    {
        Log::Warning("Font could not be loaded: " + name + " <- " + assetPath);
    }

    return loaded;
}

bool Application::SetFont(const std::string &name, float pixelHeight)
{
    // Loads the font from the default fonts folder using a shader-like API
    const std::string fontPath = std::string("assets/fonts/") + name;
    const std::string fontKey = BuildFontKey(name, pixelHeight);
    const float bakedPixelHeight = pixelHeight * FontOversampleFactor;

    if (!LoadFont(fontKey, fontPath, bakedPixelHeight))
    {
        return false;
    }

    // Makes the loaded font the default one used by UI::Print
    m_DefaultFontScale = pixelHeight / bakedPixelHeight;
    return SetDefaultFont(fontKey);
}

bool Application::SetDefaultFont(const std::string &name)
{
    // Only accepts fonts already registered in the resource manager
    if (!m_ResourceManager || !m_ResourceManager->HasFont(name))
    {
        Log::Warning("Cannot set default font. Font not found: " + name);
        return false;
    }

    m_DefaultFontName = name;
    Log::Info("Default font set: " + name);
    return true;
}

void Application::DrawText(const std::string &text, const Vec2 &position, const Color &color, float scale)
{
    if (text.empty())
    {
        return;
    }

    // Stores text for the current frame so it can be drawn after the world
    m_TextQueue.push_back({text, position, color, scale});
}

void Application::SetGravity(const Vec2 &gravity)
{
    // Updates physics gravity if physics system exists
    if (m_PhysicsSystem)
    {
        m_PhysicsSystem->SetGravity(gravity);
    }
}

Vec2 Application::GetGravity() const
{
    // Returns physics gravity if physics system exists
    if (m_PhysicsSystem)
    {
        return m_PhysicsSystem->GetGravity();
    }

    return Vec2(0.0f, 0.0f);
}

void Application::SetDebugDrawColliders(bool enabled)
{
    // Stores the global collider debug flag
    m_DebugDrawColliders = enabled;
}

bool Application::IsDebugDrawCollidersEnabled() const
{
    // Returns the current collider debug flag state
    return m_DebugDrawColliders;
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

        m_SceneManager.Update(Time::GetDeltaTime());

        // Frees entities destroyed during scene logic before physics runs
        m_Registry.FlushDestroyedEntities();

        // Toggles collider debug drawing from the keyboard
        if (Input::IsActionJustPressed("debug_colliders"))
        {
            SetDebugDrawColliders(!IsDebugDrawCollidersEnabled());
            Log::Info(std::string("Collider debug drawing ") + (IsDebugDrawCollidersEnabled() ? "enabled." : "disabled."));
        }

        // Updates physics after gameplay changes velocities
        if (m_PhysicsSystem)
        {
            m_PhysicsSystem->Update(m_Registry, Time::GetDeltaTime());
        }

        // Advances animated sprites after gameplay and physics have updated
        if (m_AnimationSystem)
        {
            m_AnimationSystem->Update(m_Registry, Time::GetDeltaTime());
        }

        // Advances particle emitters and removes expired particle entities
        if (m_ParticleSystem)
        {
            m_ParticleSystem->Update(m_Registry, Time::GetDeltaTime());
        }

        // Frees entities destroyed by physics or triggers before rendering
        m_Registry.FlushDestroyedEntities();

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

            m_SceneManager.Render();

            // Renders
            if (m_Renderer)
            {
                m_RenderSystem->Render(m_Registry, *m_ResourceManager, *m_Renderer, *shaderToUse, m_Texture.get(), m_DebugDrawColliders);
            }

            // Draws queued text overlays using the default font
            RenderQueuedText(*shaderToUse);
        }

        // Updates window
        m_Window->Update();

        // Captures the input snapshot generated by the last event poll
        Input::Update();
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
    m_AnimationSystem = std::make_unique<AnimationSystem>();
    m_ParticleSystem = std::make_unique<ParticleSystem>();
    m_PhysicsSystem = std::make_unique<PhysicsSystem>();
    m_RenderSystem = std::make_unique<RenderSystem>();
    m_SceneManager.SetPhysicsSystem(*m_PhysicsSystem);
    Log::Info("Renderer, animation system, particle system, physics system and render system created.");

    // Creates camera
    m_Camera = std::make_unique<Camera2D>(
        0.0f,
        static_cast<float>(width),
        static_cast<float>(height),
        0.0f);
    Log::Info("Main 2D camera created.");

    // Creates texture
    m_Texture = std::make_unique<Texture>();
    m_ResourceManager = std::make_unique<ResourceManager>();

    // Loads the default UI font so text works out of the box
    SetFont("PixelStormMini.ttf", 16.0f);

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
    m_ResourceManager.reset();
    m_Texture.reset();
    m_RenderSystem.reset();
    m_AnimationSystem.reset();
    m_ParticleSystem.reset();
    m_PhysicsSystem.reset();
    m_Camera.reset();
    m_Renderer.reset();
    m_EntityShader.reset();
    m_DefaultShader.reset();
    m_Window.reset();
    m_TextQueue.clear();
    m_DefaultFontName.clear();
    m_DefaultFontScale = 1.0f;

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

SceneManager& Application::GetScenes()
{
    // Returns scene manager
    return m_SceneManager;
}

const SceneManager& Application::GetScenes() const
{
    // Returns scene manager
    return m_SceneManager;
}

void Application::RenderQueuedText(Shader &shader)
{
    if (!m_Renderer || !m_ResourceManager || m_DefaultFontName.empty() || m_TextQueue.empty())
    {
        m_TextQueue.clear();
        return;
    }

    // Resolves the active font once and reuses it for every queued command
    Font *font = m_ResourceManager->GetFont(m_DefaultFontName);
    if (!font)
    {
        m_TextQueue.clear();
        return;
    }

    shader.Use();
    shader.SetInt("u_Texture", 0);

    for (const TextCommand &command : m_TextQueue)
    {
        // Draws each queued string with the shared font atlas
        m_Renderer->DrawText(shader, *font, command.Position, command.Text, command.Tint, command.Scale * m_DefaultFontScale);
    }

    m_TextQueue.clear();
}
