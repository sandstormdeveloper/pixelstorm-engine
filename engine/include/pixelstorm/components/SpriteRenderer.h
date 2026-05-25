#pragma once

#include <string>

#include <glm/vec4.hpp>

struct SpriteRenderer
{
    std::string TextureName; // Resource name used to resolve the texture
    glm::vec4 Color;         // RGBA tint multiplier
    bool Visible;            // Whether the sprite should be drawn

    SpriteRenderer();
    explicit SpriteRenderer(const glm::vec4 &color, bool visible = true);
    explicit SpriteRenderer(const std::string &textureName, const glm::vec4 &color = glm::vec4(1.0f), bool visible = true);
};
