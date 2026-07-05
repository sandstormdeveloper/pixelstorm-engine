#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

struct Transform
{
    glm::vec2 Position; // World-space position
    glm::vec2 Scale;    // Per-axis scale
    float Rotation;     // Rotation in degrees
    glm::vec2 Pivot;    // Local-space pivot used for rotation and scaling

    Transform();
    Transform(const glm::vec2 &position, const glm::vec2 &scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f, const glm::vec2 &pivot = glm::vec2(0.0f, 0.0f));

    glm::mat4 GetMatrix() const; // Builds the model matrix for this transform
};
