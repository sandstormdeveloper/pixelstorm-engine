#pragma once

#include <glm/vec2.hpp>

struct Rigidbody
{
    glm::vec2 Velocity; // Actual speed
    float Mass;         // Mass for physics
    bool IsStatic;      // If the entity should move

    Rigidbody();
    Rigidbody(const glm::vec2 &velocity, float mass = 1.0f, bool isStatic = false);
};