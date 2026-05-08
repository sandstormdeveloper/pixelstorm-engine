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
    // Horizontal movement axis
    switch (axis)
    {
    case Axis::MoveX:
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
    }

    // Returns neutral value if axis is unknown
    return 0.0f;
}

Vec2 Input::GetAxis2D(Axis2D axis)
{
    // Returns 2D movement vector
    switch (axis)
    {
    case Axis2D::Move:
    {
        Vec2 movement(
            GetAxis(Axis::MoveX),
            0.0f);

        // Vertical movement axis using screen-space coordinates
        if (IsKeyPressed(Key::S))
        {
            movement.y += 1.0f;
        }

        if (IsKeyPressed(Key::W))
        {
            movement.y -= 1.0f;
        }

        // Normalizes diagonal movement to keep the same speed in every direction
        if (movement.x != 0.0f || movement.y != 0.0f)
        {
            movement = glm::normalize(movement);
        }

        return movement;
    }
    }

    // Returns neutral value if axis is unknown
    return Vec2(0.0f, 0.0f);
}
