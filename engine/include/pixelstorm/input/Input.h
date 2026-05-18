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
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265
};

class Input
{
public:
    static void SetWindow(GLFWwindow *window); // Sets window used by input
    static void Update();                      // Updates input state for the current frame

    static bool IsKeyDown(Key key);         // Returns if key is currently held
    static bool IsKeyJustPressed(Key key);  // Returns if key was pressed this frame
    static bool IsKeyJustReleased(Key key); // Returns if key was released this frame

    static void ClearAction(const std::string &actionName);               // Removes all bindings from action
    static void AddActionBinding(const std::string &actionName, Key key); // Adds key to named action
    static bool IsActionDown(const std::string &actionName);              // Returns if action is currently held
    static bool IsActionJustPressed(const std::string &actionName);       // Returns if action was pressed this frame
    static bool IsActionJustReleased(const std::string &actionName);      // Returns if action was released this frame

    static void ClearAxis(const std::string &axisName);                                        // Removes all 1D axis bindings
    static void AddAxisBinding(const std::string &axisName, Key negativeKey, Key positiveKey); // Adds key pair to named 1D axis
    static float GetAxis(const std::string &axisName);                                         // Returns named 1D axis value

    static void ClearAxis2D(const std::string &axisName);                                                         // Removes all 2D axis bindings
    static void AddAxis2DBinding(const std::string &axisName, Key leftKey, Key rightKey, Key upKey, Key downKey); // Adds key set to named 2D axis
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

    static int ToKeyIndex(Key key);                                                         // Converts key to tracked index
    static bool IsValidKeyIndex(int key);                                                   // Returns if key index is valid
    static bool ReadKeyState(int key);                                                      // Reads key state from GLFW
    static void RegisterDefaultBindings();                                                  // Registers built-in bindings
    static bool AnyActionKeyMatches(const std::string &actionName, bool (*predicate)(Key)); // Checks if any action key matches

    static GLFWwindow *s_Window;                                                         // Window used by input
    static bool s_DefaultBindingsRegistered;                                             // Tracks if built-in bindings were created
    static std::array<unsigned char, MaxTrackedKeys> s_CurrentKeys;                      // Current key state snapshot
    static std::array<unsigned char, MaxTrackedKeys> s_PreviousKeys;                     // Previous key state snapshot
    static std::unordered_map<std::string, std::vector<Key>> s_ActionBindings;           // Action bindings
    static std::unordered_map<std::string, std::vector<AxisBinding>> s_AxisBindings;     // 1D axis bindings
    static std::unordered_map<std::string, std::vector<Axis2DBinding>> s_Axis2DBindings; // 2D axis bindings
};
