#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>

class Shader;
class Font;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void DrawQuad(const Shader &shader, const glm::mat4 &modelMatrix) const;                                                                              // Draws a quad using a model matrix
    void DrawQuadOutline(const Shader &shader, const glm::mat4 &modelMatrix) const;                                                                       // Draws a quad outline using a model matrix
    void DrawText(const Shader &shader, const Font &font, const glm::vec2 &position, const std::string &text, const glm::vec4 &color, float scale) const; // Draws text using a font atlas
    glm::vec2 MeasureText(const Font &font, const std::string &text, float scale) const;                                                                  // Measures the rendered text size in pixels

private:
    unsigned int m_VAO; // Vertex Array Object
    unsigned int m_VBO; // Vertex Buffer Object
    unsigned int m_EBO; // Element Buffer Object
};
