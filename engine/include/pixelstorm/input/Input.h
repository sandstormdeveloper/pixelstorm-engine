#pragma once

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
    Escape = 256
};

class Input
{
public:
    static void SetWindow(GLFWwindow *window); // Sets window used by GLFW
    static bool IsKeyPressed(Key key);         // Detects if a certain key is pressed
    static bool IsKeyPressed(int key);         // Legacy GLFW-compatible overload
    static bool IsKeyDown(Key key);            // Alias for IsKeyPressed

private:
    static GLFWwindow *s_Window; // Window used by GLFW
};
