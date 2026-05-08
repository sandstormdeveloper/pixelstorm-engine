#include "pixelstorm/input/Input.h"

#include "pixelstorm/core/Log.h"
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>

GLFWwindow *Input::s_Window = nullptr;

void Input::SetWindow(GLFWwindow *window)
{
    // Sets window used by GLFW
    s_Window = window;

    // Logs input binding state
    if (s_Window)
    {
        Log::Info("Input connected to GLFW window.");
    }
    else
    {
        Log::Warning("Input received a null GLFW window.");
    }
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

float Input::GetAxis(Axis axis)
{
    // Movement axes mapped to keyboard input
    switch (axis)
    {
    case Axis::Horizontal:
    {
        float axis = 0.0f;

        if (IsKeyPressed(Key::D))
        {
            axis += 1.0f;
        }

        if (IsKeyPressed(Key::A))
        {
            axis -= 1.0f;
        }

        return axis;
    }
    case Axis::Vertical:
    {
        float axis = 0.0f;

        // Screen-space coordinates: positive Y goes down
        if (IsKeyPressed(Key::S))
        {
            axis += 1.0f;
        }

        if (IsKeyPressed(Key::W))
        {
            axis -= 1.0f;
        }

        return axis;
    }
    }

    // Returns neutral value if axis is unknown
    return 0.0f;
}

Vec2 Input::GetAxis2D()
{
    // Returns 2D movement vector
    Vec2 movement(
        GetAxis(Axis::Horizontal),
        GetAxis(Axis::Vertical));

    // Normalizes diagonal movement to keep the same speed in every direction
    if (movement.x != 0.0f || movement.y != 0.0f)
    {
        movement = glm::normalize(movement);
    }

    return movement;
}
