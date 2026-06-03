#pragma once

#include <glm/vec2.hpp>

struct Collider;
struct Transform;

struct AABB
{
    glm::vec2 Min; // Lower corner of the box
    glm::vec2 Max; // Upper corner of the box

    AABB() = default;
    AABB(const glm::vec2 &min, const glm::vec2 &max)
        : Min(min),
          Max(max)
    {
    }
};

struct CollisionManifold
{
    bool IsColliding = false;           // Whether the two colliders overlap
    glm::vec2 Normal = glm::vec2(0.0f); // Direction used to separate the first body from the second
    float PenetrationDepth = 0.0f;      // Minimum distance needed to separate the bodies
};

class Collision
{
public:
    static AABB CreateAABB(const Transform &transform, const Collider &collider); // Builds an AABB from transform and collider data
    static bool Overlaps(const AABB &a, const AABB &b);                           // Returns true if two AABBs overlap
    static CollisionManifold CheckAABB(const AABB &a, const AABB &b);             // Returns overlap information for two AABBs
};
