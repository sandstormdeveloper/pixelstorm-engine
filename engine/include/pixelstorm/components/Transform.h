#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

struct Transform
{
    glm::vec2 Position; // World-space position
    glm::vec2 Scale;    // Per-axis scale
    float Rotation;     // Rotation in degrees

    Transform();
    Transform(const glm::vec2 &position, const glm::vec2 &scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f);

    glm::mat4 GetMatrix() const; // Builds the model matrix for this transform
};
