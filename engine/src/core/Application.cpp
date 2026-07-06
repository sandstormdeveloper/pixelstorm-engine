#include "pixelstorm/core/Application.h"
#include "pixelstorm/core/Log.h"
#include "pixelstorm/core/Time.h"
#include "pixelstorm/core/Window.h"
#include "pixelstorm/components/Transform.h"
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
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cmath>
#include <limits>
#include <memory>

namespace
{
    constexpr float FontOversampleFactor = 4.0f;

    std::string BuildFontKey(const std::string &name, float pixelHeight)
    {
        return name + "@" + std::to_string(static_cast<int>(std::lround(pixelHeight)));
    }

    void DestroyFramebuffer(unsigned int &framebuffer, unsigned int &colorTexture)
    {
        // Releases the offscreen color target before recreating it
        if (colorTexture != 0)
        {
            glDeleteTextures(1, &colorTexture);
            colorTexture = 0;
        }

        // Releases the framebuffer object itself
        if (framebuffer != 0)
        {
            glDeleteFramebuffers(1, &framebuffer);
            framebuffer = 0;
        }
    }

    bool CreateFramebuffer(unsigned int &framebuffer, unsigned int &colorTexture, int width, int height)
    {
        if (width <= 0 || height <= 0)
        {
            return false;
        }

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

        const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (!complete)
        {
            DestroyFramebuffer(framebuffer, colorTexture);
        }

        return complete;
    }
}

Application::Application(int width, int height, const char *title)
    : m_World(m_Registry),
      m_SceneManager(m_World),
      m_DebugDrawColliders(false),
      m_CameraFollowTarget(),
      m_CameraFollowOffset(0.0f, 0.0f),
      m_CameraFollowRotation(false),
      m_CameraFollowSpeed(8.0f),
      m_SnapCameraOnNextFollow(false),
      m_PostProcessEnabled(true),
      m_PostProcessFBO(0),
      m_PostProcessColorTexture(0),
      m_PostProcessWidth(0),
      m_PostProcessHeight(0),
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

void Application::SetPostProcessEnabled(bool enabled)
{
    // Stores whether the CRT-style screen pass should run after world rendering
    m_PostProcessEnabled = enabled;
    Log::Info(std::string("Postprocess ") + (m_PostProcessEnabled ? "enabled." : "disabled."));
}

bool Application::IsPostProcessEnabled() const
{
    // Returns the current postprocess toggle state
    return m_PostProcessEnabled;
}

void Application::DrawText(const std::string &text, const Vec2 &position, const Color &color, float scale, bool followCamera, TextAnchor anchor)
{
    if (text.empty())
    {
        return;
    }

    // Stores text for the current frame so it can be drawn after the world
    m_TextQueue.push_back({text, position, color, scale, followCamera, anchor});
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

Camera2D &Application::GetCamera()
{
    // Returns the main camera
    return *m_Camera;
}

const Camera2D &Application::GetCamera() const
{
    // Returns the main camera in read-only mode
    return *m_Camera;
}

void Application::SetCameraPosition(const Vec2 &position)
{
    // Moves the main camera
    if (m_Camera)
    {
        m_Camera->SetPosition(position);
    }
}

void Application::SetCameraRotation(float rotationDegrees)
{
    // Rotates the main camera
    if (m_Camera)
    {
        m_Camera->SetRotation(rotationDegrees);
    }
}

void Application::SetCameraProjection(float left, float right, float bottom, float top)
{
    // Updates the main camera bounds
    if (m_Camera)
    {
        m_Camera->SetProjection(left, right, bottom, top);
    }
}

void Application::FollowCamera(Entity entity, const Vec2 &offset, bool followRotation, float followSpeed)
{
    // Stores the entity that will drive the camera
    if (!entity.IsValid())
    {
        StopCameraFollow();
        return;
    }

    m_CameraFollowTarget = entity;
    m_CameraFollowOffset = offset;
    m_CameraFollowRotation = followRotation;
    m_CameraFollowSpeed = followSpeed;

    // A new scene can request an immediate snap on the first follow so the old camera state does not leak through
    if (m_SnapCameraOnNextFollow && m_Camera && m_CameraFollowTarget.HasComponent<Transform>())
    {
        const Transform &transform = m_CameraFollowTarget.GetComponent<Transform>();
        const Vec2 targetPosition = transform.Position + m_CameraFollowOffset;
        m_Camera->SetPosition(targetPosition);

        if (m_CameraFollowRotation)
        {
            m_Camera->SetRotation(transform.Rotation);
        }

        m_SnapCameraOnNextFollow = false;
    }
}

void Application::StopCameraFollow()
{
    // Clears the camera follow target
    m_CameraFollowTarget = Entity();
    m_CameraFollowOffset = Vec2(0.0f, 0.0f);
    m_CameraFollowRotation = false;
    m_CameraFollowSpeed = 8.0f;
}

void Application::ResetCameraTracking()
{
    // Restores the default camera state and asks the next follow to snap immediately
    StopCameraFollow();
    SetCameraPosition(Vec2(
        static_cast<float>(Window::GetLogicalWidth()) * 0.5f,
        static_cast<float>(Window::GetLogicalHeight()) * 0.5f));
    SetCameraRotation(0.0f);
    m_SnapCameraOnNextFollow = true;
}

bool Application::IsCameraFollowing() const
{
    // Returns whether a valid entity is driving the camera
    return m_CameraFollowTarget.IsValid();
}

bool Application::IsPositionOutsideCamera(const Vec2 &position, float margin) const
{
    // Returns false if no camera is available yet
    if (!m_Camera)
    {
        return false;
    }

    // Transforms the world position into clip space using the active camera
    const glm::vec4 clipPosition = m_Camera->GetViewProjectionMatrix() * glm::vec4(position, 0.0f, 1.0f);
    if (clipPosition.w == 0.0f)
    {
        return true;
    }

    const glm::vec3 ndcPosition = glm::vec3(clipPosition) / clipPosition.w;
    const float marginX = Window::GetLogicalWidth() > 0 ? margin / (static_cast<float>(Window::GetLogicalWidth()) * 0.5f) : 0.0f;
    const float marginY = Window::GetLogicalHeight() > 0 ? margin / (static_cast<float>(Window::GetLogicalHeight()) * 0.5f) : 0.0f;

    // Checks whether the position lies outside the camera view bounds
    return ndcPosition.x < -1.0f - marginX ||
           ndcPosition.x > 1.0f + marginX ||
           ndcPosition.y < -1.0f - marginY ||
           ndcPosition.y > 1.0f + marginY;
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

        // Keeps the offscreen scene buffer aligned with the current framebuffer size
        const int framebufferWidth = Window::GetFramebufferWidth() > 0 ? Window::GetFramebufferWidth() : 1;
        const int framebufferHeight = Window::GetFramebufferHeight() > 0 ? Window::GetFramebufferHeight() : 1;
        if (m_PostProcessEnabled)
        {
            EnsurePostProcessTarget(framebufferWidth, framebufferHeight);
        }
        const bool postProcessTargetReady = m_PostProcessFBO != 0 && m_PostProcessColorTexture != 0 && m_PostProcessWidth > 0 && m_PostProcessHeight > 0;
        const bool renderToPostProcess = m_PostProcessEnabled && postProcessTargetReady;

        // Decides shader to use for the world and UI pass
        Shader *shaderToUse = GetActiveShader();

        // Renders the scene to the offscreen target so the final pass can add CRT effects
        glBindFramebuffer(GL_FRAMEBUFFER, renderToPostProcess ? m_PostProcessFBO : 0);
        glViewport(0, 0, renderToPostProcess ? m_PostProcessWidth : framebufferWidth, renderToPostProcess ? m_PostProcessHeight : framebufferHeight);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activates shader if one is available
        if (shaderToUse)
        {
            shaderToUse->Use();

            // Defines what texture to use
            shaderToUse->SetInt("u_Texture", 0);

            // Updates the camera before rendering if it is following an entity
            UpdateCameraFollow();

            // Sets the camera matrix for world-space text and sprites
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

        // Returns to the default framebuffer for the final screen pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (renderToPostProcess)
        {
            RenderPostProcess();
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
    m_SceneManager.SetApplication(*this);
    Log::Info("Renderer, animation system, particle system, physics system and render system created.");

    // Creates camera
    m_Camera = std::make_unique<Camera2D>(
        -static_cast<float>(width) * 0.5f,
        static_cast<float>(width) * 0.5f,
        static_cast<float>(height) * 0.5f,
        -static_cast<float>(height) * 0.5f);
    m_Camera->SetPosition(Vec2(static_cast<float>(width) * 0.5f, static_cast<float>(height) * 0.5f));
    Log::Info("Main 2D camera created.");

    // Creates texture
    m_Texture = std::make_unique<Texture>();
    m_ResourceManager = std::make_unique<ResourceManager>();

    // Loads the default UI font so text works out of the box
    const std::string defaultFontName = "PixelStormMini.ttf";
    const std::string defaultFontPath = std::string("assets/fonts/") + defaultFontName;
    const float defaultFontSize = 16.0f;
    const std::string defaultFontKey = BuildFontKey(defaultFontName, defaultFontSize);
    const float defaultBakedFontSize = defaultFontSize * FontOversampleFactor;

    if (LoadFont(defaultFontKey, defaultFontPath, defaultBakedFontSize))
    {
        m_DefaultFontScale = defaultFontSize / defaultBakedFontSize;
        SetDefaultFont(defaultFontKey);
    }
    else
    {
        Log::Warning("Default UI font could not be loaded.");
    }

    // Sets default shaders (can be overwritten)
    m_DefaultShader = std::make_unique<Shader>("default");
    m_EntityShader.reset();
    m_PostProcessShader = std::make_unique<Shader>("crt");
    EnsurePostProcessTarget(Window::GetFramebufferWidth(), Window::GetFramebufferHeight());
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
    m_PostProcessShader.reset();
    DestroyPostProcessTarget();
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

void Application::UpdateCameraFollow()
{
    // Keeps the camera attached to the tracked entity while it remains valid
    if (!m_Camera || !m_CameraFollowTarget.IsValid())
    {
        return;
    }

    if (!m_CameraFollowTarget.HasComponent<Transform>())
    {
        StopCameraFollow();
        return;
    }

    const Transform &transform = m_CameraFollowTarget.GetComponent<Transform>();
    const Vec2 targetPosition = transform.Position + m_CameraFollowOffset;
    const Vec2 currentPosition = m_Camera->GetPosition();

    if (m_CameraFollowSpeed <= 0.0f)
    {
        // Zero or negative speed keeps the old instant-follow behavior
        m_Camera->SetPosition(targetPosition);
    }
    else
    {
        // Smoothly approaches the target position to create a small follow delay
        const float deltaTime = static_cast<float>(Time::GetDeltaTime());
        const float followFactor = 1.0f - std::exp(-m_CameraFollowSpeed * deltaTime);
        m_Camera->SetPosition(currentPosition + (targetPosition - currentPosition) * followFactor);
    }

    if (m_CameraFollowRotation)
    {
        const float currentRotation = m_Camera->GetRotation();
        if (m_CameraFollowSpeed <= 0.0f)
        {
            // Zero or negative speed keeps the old instant-follow behavior
            m_Camera->SetRotation(transform.Rotation);
        }
        else
        {
            // Smoothly approaches the target rotation to match the positional delay
            const float deltaTime = static_cast<float>(Time::GetDeltaTime());
            const float followFactor = 1.0f - std::exp(-m_CameraFollowSpeed * deltaTime);
            m_Camera->SetRotation(currentRotation + (transform.Rotation - currentRotation) * followFactor);
        }
    }
}

void Application::EnsurePostProcessTarget(int width, int height)
{
    // Normalizes invalid sizes so the framebuffer always has a usable target
    if (width <= 0 || height <= 0)
    {
        width = 1;
        height = 1;
    }

    // Keeps the current target if it already matches the active framebuffer size
    if (m_PostProcessFBO != 0 && m_PostProcessWidth == width && m_PostProcessHeight == height)
    {
        return;
    }

    // Recreates the offscreen buffer when the window framebuffer changes size
    DestroyPostProcessTarget();
    m_PostProcessWidth = width;
    m_PostProcessHeight = height;

    if (!CreateFramebuffer(m_PostProcessFBO, m_PostProcessColorTexture, width, height))
    {
        m_PostProcessWidth = 0;
        m_PostProcessHeight = 0;
        Log::Error("Failed to create the postprocess framebuffer.");
    }
    else
    {
        Log::Info("Postprocess framebuffer created at " + std::to_string(width) + "x" + std::to_string(height) + ".");
    }
}

void Application::DestroyPostProcessTarget()
{
    // Releases the offscreen scene target if one exists
    DestroyFramebuffer(m_PostProcessFBO, m_PostProcessColorTexture);
    m_PostProcessWidth = 0;
    m_PostProcessHeight = 0;
}

void Application::RenderPostProcess()
{
    if (!m_PostProcessEnabled || !m_Renderer || !m_PostProcessShader || m_PostProcessColorTexture == 0 || m_PostProcessWidth <= 0 || m_PostProcessHeight <= 0)
    {
        // Falls back to a plain swap if the postprocess resources are not ready
        return;
    }

    // Restores the framebuffer viewport for the final full-screen pass
    glViewport(0, 0, m_PostProcessWidth, m_PostProcessHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_PostProcessShader->Use();
    m_PostProcessShader->SetInt("u_Texture", 0);
    m_PostProcessShader->SetFloat("u_Time", static_cast<float>(Time::GetElapsedTime()));
    m_PostProcessShader->SetMat4("u_ViewProjection", glm::mat4(1.0f));

    // Draws the offscreen scene texture over the whole screen with the CRT pass
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_PostProcessColorTexture);
    const glm::mat4 fullScreenModel = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));
    m_Renderer->DrawQuad(*m_PostProcessShader, fullScreenModel);
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

    const glm::mat4 screenProjection = glm::ortho(
        0.0f,
        static_cast<float>(Window::GetLogicalWidth()),
        static_cast<float>(Window::GetLogicalHeight()),
        0.0f,
        -1.0f,
        1.0f);

    for (const TextCommand &command : m_TextQueue)
    {
        // Chooses whether the text follows the camera or stays fixed to the screen
        if (m_Camera)
        {
            shader.SetMat4("u_ViewProjection", command.FollowCamera ? m_Camera->GetViewProjectionMatrix() : screenProjection);
        }

        // Centers or aligns the text block before drawing if requested
        glm::vec2 drawPosition = command.Position;
        const glm::vec2 measuredSize = m_Renderer->MeasureText(*font, command.Text, command.Scale * m_DefaultFontScale);

        switch (command.Anchor)
        {
        case TextAnchor::TopCenter:
        case TextAnchor::MiddleCenter:
        case TextAnchor::BottomCenter:
            drawPosition.x -= measuredSize.x * 0.5f;
            break;
        case TextAnchor::TopRight:
        case TextAnchor::MiddleRight:
        case TextAnchor::BottomRight:
            drawPosition.x -= measuredSize.x;
            break;
        default:
            break;
        }

        switch (command.Anchor)
        {
        case TextAnchor::MiddleLeft:
        case TextAnchor::MiddleCenter:
        case TextAnchor::MiddleRight:
            drawPosition.y -= measuredSize.y * 0.5f;
            break;
        case TextAnchor::BottomLeft:
        case TextAnchor::BottomCenter:
        case TextAnchor::BottomRight:
            drawPosition.y -= measuredSize.y;
            break;
        default:
            break;
        }

        // Draws each queued string with the shared font atlas
        m_Renderer->DrawText(shader, *font, drawPosition, command.Text, command.Tint, command.Scale * m_DefaultFontScale);
    }

    m_TextQueue.clear();
}
