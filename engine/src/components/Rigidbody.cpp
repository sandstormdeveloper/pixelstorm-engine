#include "pixelstorm/components/Rigidbody.h"

Rigidbody::Rigidbody()
    : Velocity(0.0f, 0.0f),
      IsStatic(false),
      UseGravity(false),
      GravityScale(25.0f)
{
}

Rigidbody::Rigidbody(const glm::vec2 &velocity, bool isStatic, bool useGravity, float gravityScale)
    : Velocity(velocity),
      IsStatic(isStatic),
      UseGravity(useGravity),
      GravityScale(gravityScale)
{
}
