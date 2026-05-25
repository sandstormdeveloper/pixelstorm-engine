#pragma once

#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/ecs/World.h"

#include <functional>
#include <memory>
#include <string>

class Camera2D;
class Renderer;
class RenderSystem;
class ResourceManager;
class Shader;
class Texture;
class Window;

class Application
{
public:
    Application(int width, int height, const char *title);
    ~Application();

    void Run();                                                              // Game loop
    void SetDefaultShader(const std::string &name);                          // Sets default shader used
    bool LoadTexture(const std::string &name, const std::string &assetPath); // Loads texture from runtime assets path

    void OnUpdate(const std::function<void(float)> &callback); // Passes update function to application

    World &GetWorld();             // Returns game world handle
    const World &GetWorld() const; // Returns read-only game world handle

private:
    void Init(int width, int height, const char *title); // Initializes application
    void Shutdown();                                     // Shuts down application

    Shader *GetActiveShader() const; // Returns active shader

    std::unique_ptr<Window> m_Window;                   // Main window
    std::unique_ptr<Renderer> m_Renderer;               // Basic 2D renderer
    std::unique_ptr<Camera2D> m_Camera;                 // Main 2D camera
    std::unique_ptr<Shader> m_DefaultShader;            // Base shader
    std::unique_ptr<Shader> m_EntityShader;             // Shader for objects
    std::unique_ptr<Texture> m_Texture;                 // Fallback procedural texture
    std::unique_ptr<ResourceManager> m_ResourceManager; // Loaded resources
    std::unique_ptr<RenderSystem> m_RenderSystem;       // Render system

    Registry m_Registry; // Entity registry
    World m_World;       // Public game world handle

    std::function<void(float)> m_UpdateCallback; // Update callback
};
