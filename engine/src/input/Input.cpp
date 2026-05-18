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

float Input::GetAxis(Axis axis, AxisMapping mapping)
{
    Key positiveKey = Key::Right;
    Key negativeKey = Key::Left;

    // Chooses input mapping for the requested axis
    switch (mapping)
    {
    case AxisMapping::Arrows:
        if (axis == Axis::Horizontal)
        {
            positiveKey = Key::Right;
            negativeKey = Key::Left;
        }
        else
        {
            positiveKey = Key::Down;
            negativeKey = Key::Up;
        }
        break;
    case AxisMapping::WASD:
        if (axis == Axis::Horizontal)
        {
            positiveKey = Key::D;
            negativeKey = Key::A;
        }
        else
        {
            positiveKey = Key::S;
            negativeKey = Key::W;
        }
        break;
    }

    // Movement axes mapped to keyboard input
    switch (axis)
    {
    case Axis::Horizontal:
    case Axis::Vertical:
    {
        float axisValue = 0.0f;

        // Screen-space coordinates: positive Y goes down
        if (IsKeyPressed(positiveKey))
        {
            axisValue += 1.0f;
        }

        if (IsKeyPressed(negativeKey))
        {
            axisValue -= 1.0f;
        }

        return axisValue;
    }
    }

    // Returns neutral value if axis is unknown
    return 0.0f;
}

Vec2 Input::GetAxis2D(AxisMapping mapping)
{
    // Returns 2D movement vector
    Vec2 movement(
        GetAxis(Axis::Horizontal, mapping),
        GetAxis(Axis::Vertical, mapping));

    // Normalizes diagonal movement to keep the same speed in every direction
    if (movement.x != 0.0f || movement.y != 0.0f)
    {
        movement = glm::normalize(movement);
    }

    return movement;
}
