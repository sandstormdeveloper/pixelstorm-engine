#include "pixelstorm/components/Rigidbody.h"

Rigidbody::Rigidbody()
    : Velocity(0.0f, 0.0f),
      Mass(1.0f),
      IsStatic(false)
{
}

Rigidbody::Rigidbody(const glm::vec2 &velocity, float mass, bool isStatic)
    : Velocity(velocity),
      Mass(mass),
      IsStatic(isStatic)
{
}