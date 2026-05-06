#include "pixelstorm/input/Input.h"

GLFWwindow *Input::s_Window = nullptr;

void Input::SetWindow(GLFWwindow *window)
{
    // Sets window used by GLFW
    s_Window = window;
}

bool Input::IsKeyPressed(int key)
{
    // Detects if a certain key is pressed
    return glfwGetKey(s_Window, key) == GLFW_PRESS;
}
