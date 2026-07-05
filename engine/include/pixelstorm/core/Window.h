#pragma once
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char *title);
    ~Window();

    void Update();            // Updates window
    bool ShouldClose() const; // Returns if the window should close

    GLFWwindow *GetNativeWindow() const; // Getter for m_Window
    static int GetLogicalWidth();        // Returns the logical rendering width
    static int GetLogicalHeight();       // Returns the logical rendering height
    static int GetFramebufferWidth();    // Returns the current framebuffer width
    static int GetFramebufferHeight();   // Returns the current framebuffer height

private:
    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height); // Adjusts viewport when windows is resized

    GLFWwindow *m_Window; // Pointer to GLFW window

    static int s_LogicalWidth;      // Logical rendering width
    static int s_LogicalHeight;     // Logical rendering height
    static int s_FramebufferWidth;  // Current framebuffer width
    static int s_FramebufferHeight; // Current framebuffer height
};
