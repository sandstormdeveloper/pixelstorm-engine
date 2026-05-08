#pragma once

#include <glm/vec4.hpp>

class Texture;

struct SpriteRenderer
{
    Texture *TextureResource; // Texture reference
    glm::vec4 Color;          // RGBA tint multiplier
    bool Visible;             // Whether the sprite should be drawn

    SpriteRenderer();
    explicit SpriteRenderer(const glm::vec4 &color, bool visible = true);
    explicit SpriteRenderer(Texture *texture, const glm::vec4 &color = glm::vec4(1.0f), bool visible = true);
};
