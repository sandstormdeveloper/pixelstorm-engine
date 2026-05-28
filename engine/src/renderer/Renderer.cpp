#include "pixelstorm/renderer/Renderer.h"

#include "pixelstorm/renderer/Shader.h"
#include <glad/glad.h>

Renderer::Renderer()
    : m_VAO(0),
      m_VBO(0),
      m_EBO(0)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // Quad vertices with UVs
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f};

    // Quad indexes
    const unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    // Creates OpenGL buffers
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    // Configures VAO
    glBindVertexArray(m_VAO);

    // Loads vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Loads indexes to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specifies location and data of vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbinds buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer::~Renderer()
{
    // Frees EBO resources
    if (m_EBO != 0)
    {
        glDeleteBuffers(1, &m_EBO);
    }

    // Frees VBO resources
    if (m_VBO != 0)
    {
        glDeleteBuffers(1, &m_VBO);
    }

    // Frees VAO resources
    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
    }
}

void Renderer::DrawQuad(const Shader &shader, const glm::mat4 &modelMatrix) const
{
    // Uploads model transform to the active shader
    shader.SetMat4("u_Model", modelMatrix);

    // Activates VAO
    glBindVertexArray(m_VAO);

    // Draws indexes as triangles
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Desactivates VAO
    glBindVertexArray(0);
}
