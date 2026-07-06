#pragma once

#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/ecs/World.h"
#include "pixelstorm/core/Color.h"
#include "pixelstorm/core/Math.h"
#include "pixelstorm/core/Text.h"
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

    void Run();                                                                                                                                                                                // Game loop
    bool LoadTexture(const std::string &name, const std::string &assetPath);                                                                                                                   // Loads texture from runtime assets path
    bool LoadFont(const std::string &name, const std::string &assetPath, float pixelHeight);                                                                                                   // Loads font from runtime assets path
    bool SetDefaultFont(const std::string &name);                                                                                                                                              // Sets default font used for DrawText
    void SetPostProcessEnabled(bool enabled);                                                                                                                                                  // Enables or disables the CRT-style postprocess pass
    bool IsPostProcessEnabled() const;                                                                                                                                                         // Returns whether the CRT-style postprocess pass is enabled
    void DrawText(const std::string &text, const Vec2 &position, const Color &color = Colors::White(), float scale = 1.0f, bool followCamera = true, TextAnchor anchor = TextAnchor::TopLeft); // Queues text for the current frame
    void SetGravity(const Vec2 &gravity);                                                                                                                                                      // Sets physics world gravity
    Vec2 GetGravity() const;                                                                                                                                                                   // Returns physics world gravity
    void SetDebugDrawColliders(bool enabled);                                                                                                                                                  // Enables or disables collider debug rendering
    bool IsDebugDrawCollidersEnabled() const;                                                                                                                                                  // Returns collider debug rendering state
    Camera2D &GetCamera();                                                                                                                                                                     // Returns the main 2D camera
    const Camera2D &GetCamera() const;                                                                                                                                                         // Returns the main 2D camera in read-only mode
    void SetCameraPosition(const Vec2 &position);                                                                                                                                              // Moves the main camera in world space
    void SetCameraRotation(float rotationDegrees);                                                                                                                                             // Rotates the main camera around the Z axis
    void SetCameraProjection(float left, float right, float bottom, float top);                                                                                                                // Updates the main camera projection bounds
    void FollowCamera(Entity entity, const Vec2 &offset = Vec2(0.0f, 0.0f), bool followRotation = false, float followSpeed = 8.0f);                                                            // Makes the camera follow an entity
    void StopCameraFollow();                                                                                                                                                                   // Clears the current camera follow target
    bool IsCameraFollowing() const;                                                                                                                                                            // Returns whether the camera is following an entity
    bool IsPositionOutsideCamera(const Vec2 &position, float margin = 0.0f) const;                                                                                                             // Returns whether a world position is outside the visible camera area
    void ResetCameraTracking();                                                                                                                                                                // Clears camera follow and restores the default scene camera state

    void OnUpdate(const std::function<void(float)> &callback); // Passes update function to application

    World &GetWorld();             // Returns game world handle
    const World &GetWorld() const; // Returns read-only game world handle

    SceneManager &GetScenes();             // Returns created scenes
    const SceneManager &GetScenes() const; // Returns read-only created scenes

private:
    void Init(int width, int height, const char *title); // Initializes application
    void Shutdown();                                     // Shuts down application
    void UpdateCameraFollow();                           // Updates camera tracking before rendering
    void EnsurePostProcessTarget(int width, int height); // Creates or resizes the postprocess framebuffer
    void DestroyPostProcessTarget();                     // Releases the postprocess framebuffer resources
    void RenderPostProcess();                            // Draws the final CRT-style screen pass

    Shader *GetActiveShader() const;       // Returns active shader
    void RenderQueuedText(Shader &shader); // Draws queued text commands

    std::unique_ptr<Window> m_Window;                   // Main window
    std::unique_ptr<Renderer> m_Renderer;               // Basic 2D renderer
    std::unique_ptr<Camera2D> m_Camera;                 // Main 2D camera
    std::unique_ptr<Shader> m_DefaultShader;            // Base shader
    std::unique_ptr<Shader> m_EntityShader;             // Shader for objects
    std::unique_ptr<Shader> m_PostProcessShader;        // Full-screen postprocess shader
    std::unique_ptr<Texture> m_Texture;                 // Fallback procedural texture
    std::unique_ptr<ResourceManager> m_ResourceManager; // Loaded resources
    std::unique_ptr<AnimationSystem> m_AnimationSystem; // Sprite animation system
    std::unique_ptr<ParticleSystem> m_ParticleSystem;   // Particle simulation system
    std::unique_ptr<PhysicsSystem> m_PhysicsSystem;     // Physics system
    std::unique_ptr<RenderSystem> m_RenderSystem;       // Render system
    bool m_DebugDrawColliders;                          // Global collider debug flag
    Entity m_CameraFollowTarget;                        // Entity currently followed by the camera
    Vec2 m_CameraFollowOffset;                          // Offset applied while following an entity
    bool m_CameraFollowRotation;                        // Whether the camera copies entity rotation
    float m_CameraFollowSpeed;                          // How quickly the camera catches up to its target
    bool m_SnapCameraOnNextFollow;                      // Whether the next camera follow should snap immediately
    bool m_PostProcessEnabled;                          // Whether the CRT-style postprocess pass is active
    unsigned int m_PostProcessFBO;                      // Framebuffer used to render the scene
    unsigned int m_PostProcessColorTexture;             // Color texture attached to the framebuffer
    int m_PostProcessWidth;                             // Cached framebuffer width
    int m_PostProcessHeight;                            // Cached framebuffer height

    Registry m_Registry; // Entity registry
    World m_World;       // Public game world handle

    SceneManager m_SceneManager; // Scene Manager

    std::function<void(float)> m_UpdateCallback; // Update callback

    struct TextCommand
    {
        std::string Text;  // Text to draw
        Vec2 Position;     // Text position in pixels
        Color Tint;        // Text color
        float Scale;       // Text scale multiplier
        bool FollowCamera; // Whether the text should use the camera transform
        TextAnchor Anchor; // Text alignment anchor
    };

    std::string m_DefaultFontName;        // Default font used by DrawText
    float m_DefaultFontScale;             // Scale applied to the default font atlas at draw time
    std::vector<TextCommand> m_TextQueue; // Text queued for the current frame
};
