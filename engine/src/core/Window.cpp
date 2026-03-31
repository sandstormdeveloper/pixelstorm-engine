#include "pixelstorm/core/Window.h"
#include <glad/glad.h>
#include <stdexcept>

Window::Window(int width, int height, const char *title) : m_Window(nullptr)
{
    // Initializes GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    // Creates window and activates OpenGL
    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_Window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

    // Loads OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    // Defines draw zone
    glViewport(0, 0, width, height);
}

Window::~Window()
{
    // Destroys window
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    // Terminates GLFW
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
    // Asjusts viewpòrt when window is resized
    (void)window;
    glViewport(0, 0, width, height);
}