#include "pixelstorm/physics/Collision.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/components/Collider.h"


AABB Collision::CreateAABB(const Transform &transform, const Collider &collider)
{
    glm::vec2 center = transform.Position + collider.Offset;
    glm::vec2 halfSize = collider.Size * 0.5f;

    AABB box;
    box.Min = center - halfSize;
    box.Max = center + halfSize;
    return box;
}

bool Collision::Overlaps(const AABB &a, const AABB &b)
{
    return a.Min.x < b.Max.x && a.Max.x > b.Min.x && a.Min.y < b.Max.y && a.Max.y > b.Min.y;
}

CollisionManifold Collision::CheckAABB(const AABB &a, const AABB &b)
{
    if (!Overlaps(a, b)) {
        return CollisionManifold();
    }

    glm::vec2 centerA = (a.Min + a.Max) * 0.5f;
    glm::vec2 centerB = (b.Min + b.Max) * 0.5f;

    float overlapX = std::min(a.Max.x - b.Min.x, b.Max.x - a.Min.x);
    float overlapY = std::min(a.Max.y - b.Min.y, b.Max.y - a.Min.y);

    CollisionManifold manifold;

    if (overlapX < overlapY) {
        manifold.PenetrationDepth = overlapX;
        manifold.Normal = centerA.x < centerB.x ? glm::vec2(-1, 0) : glm::vec2(1, 0);
    }
    else
    {
        manifold.PenetrationDepth = overlapY;
        manifold.Normal = centerA.y < centerB.y ? glm::vec2(0, -1) : glm::vec2(0, 1);
    }
}
