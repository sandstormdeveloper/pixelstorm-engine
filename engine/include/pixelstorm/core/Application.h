#pragma once
#include <memory>
#include <string>
#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/core/Window.h"
#include "pixelstorm/renderer/Camera2D.h"
#include "pixelstorm/renderer/Renderer.h"
#include "pixelstorm/renderer/Shader.h"
#include "pixelstorm/renderer/Texture.h"
#include "pixelstorm/ecs/Registry.h"

class Application
{
public:
    Application(int width, int height, const char *title);
    ~Application();

    void Run();                                     // Game loop
    void SetDefaultShader(const std::string &name); // Sets default shader used

private:
    void Init(int width, int height, const char *title); // Initializes application
    void Shutdown();                                     // Shuts down application

    void UpdateDemo();                   // Updates entities
    void RenderEntities(Shader &shader); // Renders entities

    Shader *GetActiveShader() const; // Returns active shader

    std::unique_ptr<Window> m_Window;        // Main window
    std::unique_ptr<Renderer> m_Renderer;    // Basic 2D renderer
    std::unique_ptr<Camera2D> m_Camera;      // Main 2D camera
    std::unique_ptr<Shader> m_DefaultShader; // Base shader
    std::unique_ptr<Shader> m_EntityShader;  // Shader for objects
    std::unique_ptr<Texture> m_Texture;      // First procedural texture

    Registry m_Registry;  // Entity registry
    Entity m_DemoEntity;  // Test entity
    Entity m_DemoEntity2; // Test entity
};
