#pragma once
#include <memory>
#include <string>
#include "pixelstorm/core/Window.h"
#include "pixelstorm/renderer/Shader.h"

class Application
{
public:
    Application(int width, int height, const char *title);
    ~Application();

    void Run();                                     // Game loop
    void SetDefaultShader(const std::string &name); // Sets default shader used

private:
    void Init(int width, int height, const char *title);
    void Shutdown();
    
    Shader *GetActiveShader() const;

    std::unique_ptr<Window> m_Window;        // Main window
    std::unique_ptr<Shader> m_DefaultShader; // Base shader
    std::unique_ptr<Shader> m_EntityShader;  // Shader for objects
};
