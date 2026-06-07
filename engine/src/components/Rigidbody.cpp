#include "pixelstorm/components/Rigidbody.h"

Rigidbody::Rigidbody()
    : Velocity(0.0f, 0.0f),
      Mass(1.0f),
      IsStatic(false),
      UseGravity(false),
      GravityScale(1.0f)
{
}

Rigidbody::Rigidbody(const glm::vec2 &velocity, float mass, bool isStatic, bool useGravity, float gravityScale)
    : Velocity(velocity),
      Mass(mass),
      IsStatic(isStatic),
      UseGravity(useGravity),
      GravityScale(gravityScale)
{
}
