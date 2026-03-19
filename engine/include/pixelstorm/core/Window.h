#pragma once
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    void Update();
    bool ShouldClose() const;

private:
    GLFWwindow* m_Window;
};