#pragma once
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char *title);
    ~Window();

    void Update();            // Updates window
    bool ShouldClose() const; // Returns if the window should close

private:
    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height); // Adjusts viewport when windows is resized

    GLFWwindow *m_Window; // Pointer to GLFW window
};