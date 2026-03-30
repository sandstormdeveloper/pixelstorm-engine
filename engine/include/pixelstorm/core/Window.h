#pragma once
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char *title);
    ~Window();

    void Update();            // Updates widnow
    bool ShouldClose() const; // Returns if the window shoould close

private:
    GLFWwindow *m_Window; // Pointer top GLFW window
};