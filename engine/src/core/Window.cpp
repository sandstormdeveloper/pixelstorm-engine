#include "pixelstorm/core/Window.h"
#include "pixelstorm/core/Log.h"
#include <glad/glad.h>
#include <stdexcept>

int Window::s_LogicalWidth = 0;
int Window::s_LogicalHeight = 0;

Window::Window(int width, int height, const char *title)
    : m_Window(nullptr)
{
    // Initializes GLFW
    if (!glfwInit())
    {
        Log::Error("Failed to initialize GLFW.");
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    Log::Info("GLFW initialized.");

    // Creates window and OpenGL context
    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_Window)
    {
        Log::Error("Failed to create GLFW window.");
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }

    Log::Info("Window created successfully.");

    // Stores logical render size used by the camera and mouse mapping
    s_LogicalWidth = width;
    s_LogicalHeight = height;

    // Keeps the original aspect ratio when the window is resized
    glfwSetWindowAspectRatio(m_Window, width, height);

    // Makes the window's context current on this thread
    glfwMakeContextCurrent(m_Window);

    // Updates viewport
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

    // Loads OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Log::Error("Failed to initialize GLAD.");
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    Log::Info("GLAD initialized.");

    // Defines draw zone
    glViewport(0, 0, width, height);
}

Window::~Window()
{
    // Destroys window
    if (m_Window)
    {
        Log::Info("Destroying window.");
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    // Terminates GLFW
    Log::Info("Terminating GLFW.");
    glfwTerminate();
}

void Window::Update()
{
    // Swaps buffers and processes events
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    // Returns true if user closes the window
    return glfwWindowShouldClose(m_Window);
}

void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    // Adjusts viewport when window is resized
    (void)window;
    if (width > 0 && height > 0)
    {
        glViewport(0, 0, width, height);
        Log::Info("Viewport resized to " + std::to_string(width) + "x" + std::to_string(height) + ".");
    }
}

GLFWwindow *Window::GetNativeWindow() const
{
    // Returns GLFW window
    return m_Window;
}

int Window::GetLogicalWidth()
{
    return s_LogicalWidth;
}

int Window::GetLogicalHeight()
{
    return s_LogicalHeight;
}
