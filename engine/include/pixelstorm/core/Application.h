#pragma once

#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/ecs/World.h"
#include "pixelstorm/core/Color.h"
#include "pixelstorm/core/Math.h"
#include "pixelstorm/scene/SceneManager.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Camera2D;
class Renderer;
class AnimationSystem;
class ParticleSystem;
class PhysicsSystem;
class RenderSystem;
class ResourceManager;
class Shader;
class Texture;
class Window;
class SceneManager;

class Application
{
public:
    Application(int width, int height, const char *title);
    ~Application();

    void Run();                                                                                                             // Game loop
    void SetDefaultShader(const std::string &name);                                                                         // Sets default shader used
    bool LoadTexture(const std::string &name, const std::string &assetPath);                                                // Loads texture from runtime assets path
    bool LoadFont(const std::string &name, const std::string &assetPath, float pixelHeight);                                // Loads font from runtime assets path
    bool SetFont(const std::string &name, float pixelHeight);                                                               // Loads a font from the default fonts folder and sets it as current
    bool SetDefaultFont(const std::string &name);                                                                           // Sets default font used for DrawText
    void DrawText(const std::string &text, const Vec2 &position, const Color &color = Colors::White(), float scale = 1.0f); // Queues text for the current frame
    void SetGravity(const Vec2 &gravity);                                                                                   // Sets physics world gravity
    Vec2 GetGravity() const;                                                                                                // Returns physics world gravity
    void SetDebugDrawColliders(bool enabled);                                                                               // Enables or disables collider debug rendering
    bool IsDebugDrawCollidersEnabled() const;                                                                               // Returns collider debug rendering state

    void OnUpdate(const std::function<void(float)> &callback); // Passes update function to application

    World &GetWorld();             // Returns game world handle
    const World &GetWorld() const; // Returns read-only game world handle

    SceneManager &GetScenes();             // Returns created scenes
    const SceneManager &GetScenes() const; // Returns read-only created scenes

private:
    void Init(int width, int height, const char *title); // Initializes application
    void Shutdown();                                     // Shuts down application

    Shader *GetActiveShader() const;       // Returns active shader
    void RenderQueuedText(Shader &shader); // Draws queued text commands

    std::unique_ptr<Window> m_Window;                   // Main window
    std::unique_ptr<Renderer> m_Renderer;               // Basic 2D renderer
    std::unique_ptr<Camera2D> m_Camera;                 // Main 2D camera
    std::unique_ptr<Shader> m_DefaultShader;            // Base shader
    std::unique_ptr<Shader> m_EntityShader;             // Shader for objects
    std::unique_ptr<Texture> m_Texture;                 // Fallback procedural texture
    std::unique_ptr<ResourceManager> m_ResourceManager; // Loaded resources
    std::unique_ptr<AnimationSystem> m_AnimationSystem; // Sprite animation system
    std::unique_ptr<ParticleSystem> m_ParticleSystem;   // Particle simulation system
    std::unique_ptr<PhysicsSystem> m_PhysicsSystem;     // Physics system
    std::unique_ptr<RenderSystem> m_RenderSystem;       // Render system
    bool m_DebugDrawColliders;                          // Global collider debug flag

    Registry m_Registry; // Entity registry
    World m_World;       // Public game world handle

    SceneManager m_SceneManager; // Scene Manager

    std::function<void(float)> m_UpdateCallback; // Update callback

    struct TextCommand
    {
        std::string Text; // Text to draw
        Vec2 Position;    // Screen position in pixels
        Color Tint;       // Text color
        float Scale;      // Text scale multiplier
    };

    std::string m_DefaultFontName;        // Default font used by DrawText
    float m_DefaultFontScale;             // Scale applied to the default font atlas at draw time
    std::vector<TextCommand> m_TextQueue; // Text queued for the current frame
};
