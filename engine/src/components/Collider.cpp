#include "pixelstorm/components/Collider.h"

Collider::Collider()
    : Size(1.0f, 1.0f),
      Offset(0.0f, 0.0f),
      IsTrigger(false)
{
}

Collider::Collider(const glm::vec2 &size, const glm::vec2 &offset, bool isTrigger)
    : Size(size),
      Offset(offset),
      IsTrigger(isTrigger)
{
}