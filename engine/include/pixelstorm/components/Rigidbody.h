#pragma once

#include <glm/vec2.hpp>

struct Rigidbody
{
    glm::vec2 Velocity; // Actual speed
    bool IsStatic;      // If the entity should move
    bool UseGravity;    // If gravity should affect this body
    float GravityScale; // Multiplier applied to world gravity

    Rigidbody();
    Rigidbody(const glm::vec2 &velocity, bool isStatic = false, bool useGravity = false, float gravityScale = 25.0f);
};
