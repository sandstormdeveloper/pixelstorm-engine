#pragma once

#include "pixelstorm/core/Math.h"

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

struct GLFWwindow;

enum class Key
{
    Unknown = -1,
    Space = 32,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    Escape = 256,
    F3 = 292,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265
};

enum class MouseButton
{
    Left = 0,
    Right = 1,
    Middle = 2
};

class Input
{
public:
    static void SetWindow(GLFWwindow *window); // Sets window used by input
    static void Update();                      // Updates input state for the current frame

    static bool IsKeyDown(Key key);         // Returns if key is currently held
    static bool IsKeyJustPressed(Key key);  // Returns if key was pressed this frame
    static bool IsKeyJustReleased(Key key); // Returns if key was released this frame

    static bool IsMouseButtonDown(MouseButton button);         // Returns if mouse button is currently held
    static bool IsMouseButtonJustPressed(MouseButton button);  // Returns if mouse button was pressed this frame
    static bool IsMouseButtonJustReleased(MouseButton button); // Returns if mouse button was released this frame
    static Vec2 GetMousePosition();                            // Returns mouse position in window space
    static Vec2 GetMouseDelta();                               // Returns mouse movement since last frame

    static void ClearAction(const std::string &actionName);               // Removes all bindings from action
    static void AddActionBinding(const std::string &actionName, Key key); // Adds key to named action
    static void RemoveActionBinding(const std::string &actionName, Key key); // Removes key from named action
    static bool IsActionDown(const std::string &actionName);              // Returns if action is currently held
    static bool IsActionJustPressed(const std::string &actionName);       // Returns if action was pressed this frame
    static bool IsActionJustReleased(const std::string &actionName);      // Returns if action was released this frame

    static void ClearAxis(const std::string &axisName);                                        // Removes all 1D axis bindings
    static void AddAxisBinding(const std::string &axisName, Key negativeKey, Key positiveKey); // Adds key pair to named 1D axis
    static void RemoveAxisBinding(const std::string &axisName, Key negativeKey, Key positiveKey); // Removes key pair from named 1D axis
    static float GetAxis(const std::string &axisName);                                         // Returns named 1D axis value

    static void ClearAxis2D(const std::string &axisName);                                                         // Removes all 2D axis bindings
    static void AddAxis2DBinding(const std::string &axisName, Key leftKey, Key rightKey, Key upKey, Key downKey); // Adds key set to named 2D axis
    static void RemoveAxis2DBinding(const std::string &axisName, Key leftKey, Key rightKey, Key upKey, Key downKey); // Removes key set from named 2D axis
    static Vec2 GetAxis2D(const std::string &axisName = "move");                                                  // Returns named 2D axis value

private:
    struct AxisBinding
    {
        Key NegativeKey;
        Key PositiveKey;
    };

    struct Axis2DBinding
    {
        Key LeftKey;
        Key RightKey;
        Key UpKey;
        Key DownKey;
    };

    static constexpr int MaxTrackedKeys = 512;
    static constexpr int MaxTrackedMouseButtons = 8;

    static int ToKeyIndex(Key key);                                                         // Converts key to tracked index
    static int ToMouseButtonIndex(MouseButton button);                                      // Converts mouse button to tracked index
    static bool IsValidKeyIndex(int key);                                                   // Returns if key index is valid
    static bool IsValidMouseButtonIndex(int button);                                        // Returns if mouse button index is valid
    static bool ReadKeyState(int key);                                                      // Reads key state from GLFW
    static bool ReadMouseButtonState(int button);                                           // Reads mouse button state from GLFW
    static void RegisterDefaultBindings();                                                  // Registers built-in bindings
    static bool AnyActionKeyMatches(const std::string &actionName, bool (*predicate)(Key)); // Checks if any action key matches
    static bool HasActionBinding(const std::string &actionName, Key key);                   // Returns if action already contains key
    static bool HasAxisBinding(const std::string &axisName, Key negativeKey, Key positiveKey); // Returns if 1D axis already contains key pair
    static bool HasAxis2DBinding(const std::string &axisName, Key leftKey, Key rightKey, Key upKey, Key downKey); // Returns if 2D axis already contains key set

    static GLFWwindow *s_Window;                                                         // Window used by input
    static bool s_DefaultBindingsRegistered;                                             // Tracks if built-in bindings were created
    static std::array<unsigned char, MaxTrackedKeys> s_CurrentKeys;                      // Current key state snapshot
    static std::array<unsigned char, MaxTrackedKeys> s_PreviousKeys;                     // Previous key state snapshot
    static std::array<unsigned char, MaxTrackedMouseButtons> s_CurrentMouseButtons;      // Current mouse button state snapshot
    static std::array<unsigned char, MaxTrackedMouseButtons> s_PreviousMouseButtons;     // Previous mouse button state snapshot
    static Vec2 s_CurrentMousePosition;                                                  // Current mouse position
    static Vec2 s_PreviousMousePosition;                                                 // Previous mouse position
    static std::unordered_map<std::string, std::vector<Key>> s_ActionBindings;           // Action bindings
    static std::unordered_map<std::string, std::vector<AxisBinding>> s_AxisBindings;     // 1D axis bindings
    static std::unordered_map<std::string, std::vector<Axis2DBinding>> s_Axis2DBindings; // 2D axis bindings
};
