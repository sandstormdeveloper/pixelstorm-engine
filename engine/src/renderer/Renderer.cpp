#include "pixelstorm/renderer/Renderer.h"

#include "pixelstorm/renderer/Font.h"
#include "pixelstorm/renderer/Shader.h"

#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>
#include <cmath>
#include <cstddef>
#include <string>

namespace
{
    // Decodes one UTF-8 codepoint and advances the string cursor
    unsigned int DecodeUtf8(const std::string &text, std::size_t &index)
    {
        const unsigned char first = static_cast<unsigned char>(text[index]);

        if (first < 0x80)
        {
            ++index;
            return first;
        }

        if ((first & 0xE0) == 0xC0 && index + 1 < text.size())
        {
            const unsigned int codepoint = ((first & 0x1F) << 6) |
                                           (static_cast<unsigned char>(text[index + 1]) & 0x3F);
            index += 2;
            return codepoint;
        }

        if ((first & 0xF0) == 0xE0 && index + 2 < text.size())
        {
            const unsigned int codepoint = ((first & 0x0F) << 12) |
                                           ((static_cast<unsigned char>(text[index + 1]) & 0x3F) << 6) |
                                           (static_cast<unsigned char>(text[index + 2]) & 0x3F);
            index += 3;
            return codepoint;
        }

        if ((first & 0xF8) == 0xF0 && index + 3 < text.size())
        {
            const unsigned int codepoint = ((first & 0x07) << 18) |
                                           ((static_cast<unsigned char>(text[index + 1]) & 0x3F) << 12) |
                                           ((static_cast<unsigned char>(text[index + 2]) & 0x3F) << 6) |
                                           (static_cast<unsigned char>(text[index + 3]) & 0x3F);
            index += 4;
            return codepoint;
        }

        ++index;
        return 0xFFFD;
    }
}

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

void Renderer::DrawQuadOutline(const Shader &shader, const glm::mat4 &modelMatrix) const
{
    // Uploads model transform to the active shader
    shader.SetMat4("u_Model", modelMatrix);

    // Activates VAO
    glBindVertexArray(m_VAO);

    // Draws the quad as an outline
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    // Desactivates VAO
    glBindVertexArray(0);
}

void Renderer::DrawText(const Shader &shader, const Font &font, const glm::vec2 &position, const std::string &text, const glm::vec4 &color, float scale) const
{
    if (!font.IsValid() || text.empty())
    {
        return;
    }

    // Text reuses the same textured quad pipeline as sprites
    font.Bind(0);
    shader.SetInt("u_Texture", 0);
    shader.SetInt("u_UseTexture", 1);
    shader.SetVec4("u_Color", color);

    // Text is positioned from the top-left and converted to the font baseline internally
    const glm::vec2 start(std::round(position.x), std::round(position.y));
    glm::vec2 pen = start;
    pen.y = std::round(start.y + font.GetAscent() * scale);
    const float lineStep = std::round(font.GetLineHeight() * scale);

    for (std::size_t i = 0; i < text.size();)
    {
        const unsigned int codepoint = DecodeUtf8(text, i);

        if (codepoint == '\n')
        {
            // New line: move the pen to the next row
            pen.x = start.x;
            pen.y += lineStep;
            continue;
        }

        if (codepoint == '\r')
        {
            continue;
        }

        const Font::Glyph *glyph = font.FindGlyph(codepoint);
        if (!glyph)
        {
            glyph = font.FindGlyph('?');
        }

        if (!glyph)
        {
            continue;
        }

        // Builds a quad in pixel space from the glyph metrics
        const float glyphWidth = std::round((glyph->X1 - glyph->X0) * scale);
        const float glyphHeight = std::round((glyph->Y1 - glyph->Y0) * scale);
        const float glyphX = std::round(pen.x + glyph->X0 * scale);
        const float glyphY = std::round(pen.y + glyph->Y0 * scale);

        const glm::mat4 model =
            glm::translate(glm::mat4(1.0f), glm::vec3(glyphX + (glyphWidth * 0.5f), glyphY + (glyphHeight * 0.5f), 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(glyphWidth, glyphHeight, 1.0f));

        shader.SetVec4("u_SourceRect", glm::vec4(glyph->U0, glyph->V0, glyph->U1 - glyph->U0, glyph->V1 - glyph->V0));
        DrawQuad(shader, model);
        pen.x = std::round(pen.x + glyph->XAdvance * scale);
    }
}
