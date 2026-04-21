#include "pixelstorm/components/Transform.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

Transform::Transform()
    : Position(0.0f, 0.0f),
      Scale(1.0f, 1.0f),
      Rotation(0.0f)
{
}

Transform::Transform(const glm::vec2 &position, const glm::vec2 &scale, float rotation)
    : Position(position),
      Scale(scale),
      Rotation(rotation)
{
}

glm::mat4 Transform::GetMatrix() const
{
  // Returns model matrix
  return glm::translate(glm::mat4(1.0f), glm::vec3(Position, 0.0f)) *
         glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
         glm::scale(glm::mat4(1.0f), glm::vec3(Scale, 1.0f));
}
