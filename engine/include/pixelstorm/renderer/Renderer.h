#pragma once

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void DrawQuad() const; // Draws a quad

private:
    unsigned int m_VAO; // Vertex Array Object
    unsigned int m_VBO; // Vertex Buffer Object
    unsigned int m_EBO; // Element Buffer Object
};
