#include "pixelstorm/physics/Collision.h"
#include "pixelstorm/components/Collider.h"
#include "pixelstorm/components/Transform.h"

#include <algorithm>

AABB Collision::CreateAABB(const Transform &transform, const Collider &collider)
{
    // Calculates collider center in world space
    glm::vec2 center = transform.Position + collider.Offset;

    // Calculates half-size to expand from center to corners
    glm::vec2 halfSize = collider.Size * 0.5f;

    // Builds box corners from center and half-size
    AABB box;
    box.Min = center - halfSize;
    box.Max = center + halfSize;

    return box;
}

bool Collision::Overlaps(const AABB &a, const AABB &b)
{
    // Checks if boxes overlap on both axes
    return a.Min.x < b.Max.x &&
           a.Max.x > b.Min.x &&
           a.Min.y < b.Max.y &&
           a.Max.y > b.Min.y;
}

CollisionManifold Collision::CheckAABB(const AABB &a, const AABB &b)
{
    // Returns an empty manifold if boxes don't overlap
    if (!Overlaps(a, b))
    {
        return CollisionManifold();
    }

    // Calculates box centers
    glm::vec2 centerA = (a.Min + a.Max) * 0.5f;
    glm::vec2 centerB = (b.Min + b.Max) * 0.5f;

    // Calculates penetration amount on each axis
    float overlapX = std::min(a.Max.x - b.Min.x, b.Max.x - a.Min.x);
    float overlapY = std::min(a.Max.y - b.Min.y, b.Max.y - a.Min.y);

    // Creates collision information
    CollisionManifold manifold;
    manifold.IsColliding = true;

    // Separates on the axis with the smallest penetration
    if (overlapX < overlapY)
    {
        manifold.PenetrationDepth = overlapX;
        manifold.Normal = centerA.x < centerB.x ? glm::vec2(-1, 0) : glm::vec2(1, 0);
    }
    else
    {
        manifold.PenetrationDepth = overlapY;
        manifold.Normal = centerA.y < centerB.y ? glm::vec2(0, -1) : glm::vec2(0, 1);
    }

    return manifold;
}
