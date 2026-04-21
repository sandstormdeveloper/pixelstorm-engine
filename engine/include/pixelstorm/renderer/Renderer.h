#pragma once

#include <glm/vec2.hpp>

class Shader;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void DrawQuad(const Shader &shader, const glm::vec2 &position, const glm::vec2 &size, float rotationDegrees) const; // Draws a transformed quad

private:
    unsigned int m_VAO; // Vertex Array Object
    unsigned int m_VBO; // Vertex Buffer Object
    unsigned int m_EBO; // Element Buffer Object
};
