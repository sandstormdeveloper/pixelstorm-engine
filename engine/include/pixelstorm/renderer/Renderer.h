#pragma once

#include <glm/mat4x4.hpp>

class Shader;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void DrawQuad(const Shader &shader, const glm::mat4 &modelMatrix) const; // Draws a quad using a model matrix
    void DrawQuadOutline(const Shader &shader, const glm::mat4 &modelMatrix) const; // Draws a quad outline using a model matrix

private:
    unsigned int m_VAO; // Vertex Array Object
    unsigned int m_VBO; // Vertex Buffer Object
    unsigned int m_EBO; // Element Buffer Object
};
