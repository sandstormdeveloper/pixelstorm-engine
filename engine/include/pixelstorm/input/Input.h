#pragma once

#include <GLFW/glfw3.h>

class Input
{
public:
    static void SetWindow(GLFWwindow *window); // Sets window used by GLFW
    static bool IsKeyPressed(int key);         // Detects if a certain key is pressed

private:
    static GLFWwindow *s_Window; // Window used by GLFW
};