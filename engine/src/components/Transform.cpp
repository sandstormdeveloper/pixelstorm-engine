#include "pixelstorm/components/Transform.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

Transform::Transform()
    : Position(0.0f, 0.0f),
      Scale(1.0f, 1.0f),
      Rotation(0.0f),
      Pivot(0.0f, 0.0f)
{
}

Transform::Transform(const glm::vec2 &position, const glm::vec2 &scale, float rotation, const glm::vec2 &pivot)
    : Position(position),
      Scale(scale),
      Rotation(rotation),
      Pivot(pivot)
{
}

glm::mat4 Transform::GetMatrix() const
{
    // Returns model matrix
    return glm::translate(glm::mat4(1.0f), glm::vec3(Position, 0.0f)) *
           glm::translate(glm::mat4(1.0f), glm::vec3(Pivot, 0.0f)) *
           glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
           glm::scale(glm::mat4(1.0f), glm::vec3(Scale, 1.0f)) *
           glm::translate(glm::mat4(1.0f), glm::vec3(-Pivot, 0.0f));
}
