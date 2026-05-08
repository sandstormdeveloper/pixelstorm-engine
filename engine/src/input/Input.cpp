#include "pixelstorm/input/Input.h"

#include <GLFW/glfw3.h>

GLFWwindow *Input::s_Window = nullptr;

void Input::SetWindow(GLFWwindow *window)
{
    // Sets window used by GLFW
    s_Window = window;
}

bool Input::IsKeyPressed(Key key)
{
    // Uses engine key codes and keeps GLFW as an implementation detail
    return IsKeyPressed(static_cast<int>(key));
}

bool Input::IsKeyPressed(int key)
{
    // Detects if a certain key is pressed
    if (!s_Window)
    {
        return false;
    }

    return glfwGetKey(s_Window, key) == GLFW_PRESS;
}

bool Input::IsKeyDown(Key key)
{
    // Alias with more game-facing naming
    return IsKeyPressed(key);
}
