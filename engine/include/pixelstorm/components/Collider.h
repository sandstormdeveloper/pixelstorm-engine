#pragma once

#include <glm/vec2.hpp>

struct Collider
{
    glm::vec2 Size;   // Collider size
    glm::vec2 Offset; // Offset relative to transform
    bool IsTrigger;   // Detects collisions, but doesn't block movement

    Collider();
    Collider(const glm::vec2 &size, const glm::vec2 &offset = glm::vec2(0.0f), bool isTrigger = false);
};