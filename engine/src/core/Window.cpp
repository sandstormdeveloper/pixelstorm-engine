#include "pixelstorm/core/Window.h"
#include <glad/glad.h>
#include <iostream>

Window::Window(int width, int height, const char* title)
{
    glfwInit();

    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, width, height);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::Update()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}