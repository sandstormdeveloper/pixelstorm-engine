#include "pixelstorm/input/Input.h"

#include "pixelstorm/core/Log.h"

#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>

GLFWwindow *Input::s_Window = nullptr;
bool Input::s_DefaultBindingsRegistered = false;
std::array<unsigned char, Input::MaxTrackedKeys> Input::s_CurrentKeys = {};
std::array<unsigned char, Input::MaxTrackedKeys> Input::s_PreviousKeys = {};
std::array<unsigned char, Input::MaxTrackedMouseButtons> Input::s_CurrentMouseButtons = {};
std::array<unsigned char, Input::MaxTrackedMouseButtons> Input::s_PreviousMouseButtons = {};
Vec2 Input::s_CurrentMousePosition = Vec2(0.0f, 0.0f);
Vec2 Input::s_PreviousMousePosition = Vec2(0.0f, 0.0f);
std::unordered_map<std::string, std::vector<Key>> Input::s_ActionBindings = {};
std::unordered_map<std::string, std::vector<Input::AxisBinding>> Input::s_AxisBindings = {};
std::unordered_map<std::string, std::vector<Input::Axis2DBinding>> Input::s_Axis2DBindings = {};

void Input::SetWindow(GLFWwindow *window)
{
    // Sets window used by input
    s_Window = window;

    // Resets tracked state
    s_CurrentKeys.fill(0);
    s_PreviousKeys.fill(0);
    s_CurrentMouseButtons.fill(0);
    s_PreviousMouseButtons.fill(0);
    s_CurrentMousePosition = Vec2(0.0f, 0.0f);
    s_PreviousMousePosition = Vec2(0.0f, 0.0f);

    // Logs window binding state
    if (s_Window)
    {
        Log::Info("Input connected to GLFW window.");
    }
    else
    {
        Log::Warning("Input received a null GLFW window.");
    }

    // Registers built-in bindings
    RegisterDefaultBindings();

    // Syncs the first snapshot
    Update();
    s_PreviousKeys = s_CurrentKeys;
    s_PreviousMouseButtons = s_CurrentMouseButtons;
    s_PreviousMousePosition = s_CurrentMousePosition;
}

void Input::Update()
{
    // Stores previous key snapshot
    s_PreviousKeys = s_CurrentKeys;
    s_PreviousMouseButtons = s_CurrentMouseButtons;
    s_PreviousMousePosition = s_CurrentMousePosition;

    // Resets state if window is missing
    if (!s_Window)
    {
        s_CurrentKeys.fill(0);
        s_CurrentMouseButtons.fill(0);
        s_CurrentMousePosition = Vec2(0.0f, 0.0f);
        return;
    }

    // Reads tracked keys from GLFW
    for (int key = 0; key < MaxTrackedKeys; ++key)
    {
        s_CurrentKeys[static_cast<std::size_t>(key)] = ReadKeyState(key) ? 1 : 0;
    }

    // Reads tracked mouse buttons from GLFW
    for (int button = 0; button < MaxTrackedMouseButtons; ++button)
    {
        s_CurrentMouseButtons[static_cast<std::size_t>(button)] = ReadMouseButtonState(button) ? 1 : 0;
    }

    // Reads mouse position from GLFW
    double mouseX = 0.0;
    double mouseY = 0.0;
    glfwGetCursorPos(s_Window, &mouseX, &mouseY);
    s_CurrentMousePosition = Vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

bool Input::IsKeyDown(Key key)
{
    // Converts key to tracked index
    const int keyIndex = ToKeyIndex(key);
    if (!IsValidKeyIndex(keyIndex))
    {
        return false;
    }

    // Returns current key state
    return s_CurrentKeys[static_cast<std::size_t>(keyIndex)] != 0;
}

bool Input::IsKeyJustPressed(Key key)
{
    // Converts key to tracked index
    const int keyIndex = ToKeyIndex(key);
    if (!IsValidKeyIndex(keyIndex))
    {
        return false;
    }

    // Returns transition from up to down
    return s_CurrentKeys[static_cast<std::size_t>(keyIndex)] != 0 &&
           s_PreviousKeys[static_cast<std::size_t>(keyIndex)] == 0;
}

bool Input::IsKeyJustReleased(Key key)
{
    // Converts key to tracked index
    const int keyIndex = ToKeyIndex(key);
    if (!IsValidKeyIndex(keyIndex))
    {
        return false;
    }

    // Returns transition from down to up
    return s_CurrentKeys[static_cast<std::size_t>(keyIndex)] == 0 &&
           s_PreviousKeys[static_cast<std::size_t>(keyIndex)] != 0;
}

bool Input::IsMouseButtonDown(MouseButton button)
{
    // Converts button to tracked index
    const int buttonIndex = ToMouseButtonIndex(button);
    if (!IsValidMouseButtonIndex(buttonIndex))
    {
        return false;
    }

    // Returns current mouse button state
    return s_CurrentMouseButtons[static_cast<std::size_t>(buttonIndex)] != 0;
}

bool Input::IsMouseButtonJustPressed(MouseButton button)
{
    // Converts button to tracked index
    const int buttonIndex = ToMouseButtonIndex(button);
    if (!IsValidMouseButtonIndex(buttonIndex))
    {
        return false;
    }

    // Returns transition from up to down
    return s_CurrentMouseButtons[static_cast<std::size_t>(buttonIndex)] != 0 &&
           s_PreviousMouseButtons[static_cast<std::size_t>(buttonIndex)] == 0;
}

bool Input::IsMouseButtonJustReleased(MouseButton button)
{
    // Converts button to tracked index
    const int buttonIndex = ToMouseButtonIndex(button);
    if (!IsValidMouseButtonIndex(buttonIndex))
    {
        return false;
    }

    // Returns transition from down to up
    return s_CurrentMouseButtons[static_cast<std::size_t>(buttonIndex)] == 0 &&
           s_PreviousMouseButtons[static_cast<std::size_t>(buttonIndex)] != 0;
}

Vec2 Input::GetMousePosition()
{
    // Returns current mouse position
    return s_CurrentMousePosition;
}

Vec2 Input::GetMouseDelta()
{
    // Returns mouse movement since last frame
    return s_CurrentMousePosition - s_PreviousMousePosition;
}

void Input::ClearAction(const std::string &actionName)
{
    // Removes action bindings
    s_ActionBindings.erase(actionName);
}

void Input::AddActionBinding(const std::string &actionName, Key key)
{
    // Ensures built-in bindings exist
    RegisterDefaultBindings();

    // Avoids duplicate bindings
    if (HasActionBinding(actionName, key))
    {
        return;
    }

    // Adds key to action
    s_ActionBindings[actionName].push_back(key);
}

void Input::RemoveActionBinding(const std::string &actionName, Key key)
{
    // Gets bindings for requested action
    std::unordered_map<std::string, std::vector<Key>>::iterator iterator = s_ActionBindings.find(actionName);
    if (iterator == s_ActionBindings.end())
    {
        return;
    }

    // Removes matching key
    std::vector<Key> &bindings = iterator->second;
    bindings.erase(
        std::remove(bindings.begin(), bindings.end(), key),
        bindings.end());

    // Removes empty action entry
    if (bindings.empty())
    {
        s_ActionBindings.erase(iterator);
    }
}

bool Input::IsActionDown(const std::string &actionName)
{
    // Returns if any bound key is held
    return AnyActionKeyMatches(actionName, &Input::IsKeyDown);
}

bool Input::IsActionJustPressed(const std::string &actionName)
{
    // Returns if any bound key was pressed this frame
    return AnyActionKeyMatches(actionName, &Input::IsKeyJustPressed);
}

bool Input::IsActionJustReleased(const std::string &actionName)
{
    // Returns if any bound key was released this frame
    return AnyActionKeyMatches(actionName, &Input::IsKeyJustReleased);
}

void Input::ClearAxis(const std::string &axisName)
{
    // Removes 1D axis bindings
    s_AxisBindings.erase(axisName);
}

void Input::AddAxisBinding(const std::string &axisName, Key negativeKey, Key positiveKey)
{
    // Ensures built-in bindings exist
    RegisterDefaultBindings();

    // Avoids duplicate bindings
    if (HasAxisBinding(axisName, negativeKey, positiveKey))
    {
        return;
    }

    // Adds key pair to 1D axis
    s_AxisBindings[axisName].push_back({negativeKey, positiveKey});
}

void Input::RemoveAxisBinding(const std::string &axisName, Key negativeKey, Key positiveKey)
{
    // Gets bindings for requested axis
    std::unordered_map<std::string, std::vector<AxisBinding>>::iterator iterator = s_AxisBindings.find(axisName);
    if (iterator == s_AxisBindings.end())
    {
        return;
    }

    // Removes matching key pair
    std::vector<AxisBinding> &bindings = iterator->second;
    bindings.erase(
        std::remove_if(
            bindings.begin(),
            bindings.end(),
            [negativeKey, positiveKey](const AxisBinding &binding) {
                return binding.NegativeKey == negativeKey &&
                       binding.PositiveKey == positiveKey;
            }),
        bindings.end());

    // Removes empty axis entry
    if (bindings.empty())
    {
        s_AxisBindings.erase(iterator);
    }
}

float Input::GetAxis(const std::string &axisName)
{
    // Ensures built-in bindings exist
    RegisterDefaultBindings();

    // Gets bindings for requested axis
    const std::unordered_map<std::string, std::vector<AxisBinding>>::const_iterator iterator = s_AxisBindings.find(axisName);
    if (iterator == s_AxisBindings.end())
    {
        return 0.0f;
    }

    // Accumulates axis value
    float axisValue = 0.0f;
    for (const AxisBinding &binding : iterator->second)
    {
        if (IsKeyDown(binding.PositiveKey))
        {
            axisValue += 1.0f;
        }

        if (IsKeyDown(binding.NegativeKey))
        {
            axisValue -= 1.0f;
        }
    }

    // Clamps axis to the expected range
    if (axisValue > 1.0f)
    {
        axisValue = 1.0f;
    }
    else if (axisValue < -1.0f)
    {
        axisValue = -1.0f;
    }

    return axisValue;
}

void Input::ClearAxis2D(const std::string &axisName)
{
    // Removes 2D axis bindings
    s_Axis2DBindings.erase(axisName);
}

void Input::AddAxis2DBinding(const std::string &axisName, Key leftKey, Key rightKey, Key upKey, Key downKey)
{
    // Ensures built-in bindings exist
    RegisterDefaultBindings();

    // Avoids duplicate bindings
    if (HasAxis2DBinding(axisName, leftKey, rightKey, upKey, downKey))
    {
        return;
    }

    // Adds key set to 2D axis
    s_Axis2DBindings[axisName].push_back({leftKey, rightKey, upKey, downKey});
}

void Input::RemoveAxis2DBinding(const std::string &axisName, Key leftKey, Key rightKey, Key upKey, Key downKey)
{
    // Gets bindings for requested axis
    std::unordered_map<std::string, std::vector<Axis2DBinding>>::iterator iterator = s_Axis2DBindings.find(axisName);
    if (iterator == s_Axis2DBindings.end())
    {
        return;
    }

    // Removes matching key set
    std::vector<Axis2DBinding> &bindings = iterator->second;
    bindings.erase(
        std::remove_if(
            bindings.begin(),
            bindings.end(),
            [leftKey, rightKey, upKey, downKey](const Axis2DBinding &binding) {
                return binding.LeftKey == leftKey &&
                       binding.RightKey == rightKey &&
                       binding.UpKey == upKey &&
                       binding.DownKey == downKey;
            }),
        bindings.end());

    // Removes empty axis entry
    if (bindings.empty())
    {
        s_Axis2DBindings.erase(iterator);
    }
}

Vec2 Input::GetAxis2D(const std::string &axisName)
{
    // Ensures built-in bindings exist
    RegisterDefaultBindings();

    // Gets bindings for requested axis
    const std::unordered_map<std::string, std::vector<Axis2DBinding>>::const_iterator iterator = s_Axis2DBindings.find(axisName);
    if (iterator == s_Axis2DBindings.end())
    {
        return Vec2(0.0f, 0.0f);
    }

    // Accumulates axis value
    Vec2 movement(0.0f, 0.0f);
    for (const Axis2DBinding &binding : iterator->second)
    {
        if (IsKeyDown(binding.RightKey))
        {
            movement.x += 1.0f;
        }

        if (IsKeyDown(binding.LeftKey))
        {
            movement.x -= 1.0f;
        }

        // Screen-space coordinates: positive Y goes down
        if (IsKeyDown(binding.DownKey))
        {
            movement.y += 1.0f;
        }

        if (IsKeyDown(binding.UpKey))
        {
            movement.y -= 1.0f;
        }
    }

    // Clamps horizontal axis
    if (movement.x > 1.0f)
    {
        movement.x = 1.0f;
    }
    else if (movement.x < -1.0f)
    {
        movement.x = -1.0f;
    }

    // Clamps vertical axis
    if (movement.y > 1.0f)
    {
        movement.y = 1.0f;
    }
    else if (movement.y < -1.0f)
    {
        movement.y = -1.0f;
    }

    // Normalizes diagonal movement to keep the same speed in every direction
    if (movement.x != 0.0f || movement.y != 0.0f)
    {
        movement = glm::normalize(movement);
    }

    return movement;
}

int Input::ToKeyIndex(Key key)
{
    // Converts engine key to integer code
    return static_cast<int>(key);
}

int Input::ToMouseButtonIndex(MouseButton button)
{
    // Converts engine mouse button to integer code
    return static_cast<int>(button);
}

bool Input::IsValidKeyIndex(int key)
{
    // Returns if key can be tracked
    return key >= 0 && key < MaxTrackedKeys;
}

bool Input::IsValidMouseButtonIndex(int button)
{
    // Returns if mouse button can be tracked
    return button >= 0 && button < MaxTrackedMouseButtons;
}

bool Input::ReadKeyState(int key)
{
    // Rejects invalid input backend or key code
    if (!s_Window || !IsValidKeyIndex(key))
    {
        return false;
    }

    // Reads key state from GLFW
    return glfwGetKey(s_Window, key) == GLFW_PRESS;
}

bool Input::ReadMouseButtonState(int button)
{
    // Rejects invalid input backend or mouse button code
    if (!s_Window || !IsValidMouseButtonIndex(button))
    {
        return false;
    }

    // Reads mouse button state from GLFW
    return glfwGetMouseButton(s_Window, button) == GLFW_PRESS;
}

void Input::RegisterDefaultBindings()
{
    // Avoids registering defaults twice
    if (s_DefaultBindingsRegistered)
    {
        return;
    }

    // Marks defaults as ready
    s_DefaultBindingsRegistered = true;

    // Built-in named axes for common keyboard layouts
    AddAxisBinding("arrows_horizontal", Key::Left, Key::Right);
    AddAxisBinding("arrows_vertical", Key::Up, Key::Down);
    AddAxisBinding("wasd_horizontal", Key::A, Key::D);
    AddAxisBinding("wasd_vertical", Key::W, Key::S);

    // The default movement axis accepts both arrow keys and WASD
    AddAxis2DBinding("move", Key::Left, Key::Right, Key::Up, Key::Down);
    AddAxis2DBinding("move", Key::A, Key::D, Key::W, Key::S);
    AddAxis2DBinding("arrows_move", Key::Left, Key::Right, Key::Up, Key::Down);
    AddAxis2DBinding("wasd_move", Key::A, Key::D, Key::W, Key::S);

    // A small set of default actions is enough to make the API immediately useful
    AddActionBinding("accept", Key::Space);
    AddActionBinding("cancel", Key::Escape);
    AddActionBinding("jump", Key::Space);
    AddActionBinding("interact", Key::E);
    AddActionBinding("pause", Key::Escape);
    AddActionBinding("debug_colliders", Key::F3);
}

bool Input::AnyActionKeyMatches(const std::string &actionName, bool (*predicate)(Key))
{
    // Ensures built-in bindings exist
    RegisterDefaultBindings();

    // Gets bindings for requested action
    const std::unordered_map<std::string, std::vector<Key>>::const_iterator iterator = s_ActionBindings.find(actionName);
    if (iterator == s_ActionBindings.end())
    {
        return false;
    }

    // Returns true if any key matches
    for (Key key : iterator->second)
    {
        if (predicate(key))
        {
            return true;
        }
    }

    return false;
}

bool Input::HasActionBinding(const std::string &actionName, Key key)
{
    // Gets bindings for requested action
    const std::unordered_map<std::string, std::vector<Key>>::const_iterator iterator = s_ActionBindings.find(actionName);
    if (iterator == s_ActionBindings.end())
    {
        return false;
    }

    // Returns if action already contains key
    return std::find(iterator->second.begin(), iterator->second.end(), key) != iterator->second.end();
}

bool Input::HasAxisBinding(const std::string &axisName, Key negativeKey, Key positiveKey)
{
    // Gets bindings for requested axis
    const std::unordered_map<std::string, std::vector<AxisBinding>>::const_iterator iterator = s_AxisBindings.find(axisName);
    if (iterator == s_AxisBindings.end())
    {
        return false;
    }

    // Returns if axis already contains key pair
    return std::find_if(
               iterator->second.begin(),
               iterator->second.end(),
               [negativeKey, positiveKey](const AxisBinding &binding) {
                   return binding.NegativeKey == negativeKey &&
                          binding.PositiveKey == positiveKey;
               }) != iterator->second.end();
}

bool Input::HasAxis2DBinding(const std::string &axisName, Key leftKey, Key rightKey, Key upKey, Key downKey)
{
    // Gets bindings for requested axis
    const std::unordered_map<std::string, std::vector<Axis2DBinding>>::const_iterator iterator = s_Axis2DBindings.find(axisName);
    if (iterator == s_Axis2DBindings.end())
    {
        return false;
    }

    // Returns if axis already contains key set
    return std::find_if(
               iterator->second.begin(),
               iterator->second.end(),
               [leftKey, rightKey, upKey, downKey](const Axis2DBinding &binding) {
                   return binding.LeftKey == leftKey &&
                          binding.RightKey == rightKey &&
                          binding.UpKey == upKey &&
                          binding.DownKey == downKey;
               }) != iterator->second.end();
}
