#include "pixelstorm/core/Window.h"
#include <glad/glad.h>

Window::Window(int width, int height, const char* title)
{
    // Initializes GLFW
    glfwInit();

    // Creates window and activates OpenGL
    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);

    // Loads OpenGL functions
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Defines draw zone
    glViewport(0, 0, width, height);
}

Window::~Window()
{
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