#pragma once

struct Collider;
struct Rigidbody;
struct Transform;

class Physics
{
public:
    static void Integrate(Transform &transform, const Rigidbody &rigidbody, float deltaTime);                                                                                                   // Applies velocity to transform
    static void ResolveAABB(Transform &dynamicTransform, Rigidbody &dynamicBody, const Transform &staticTransform, const Collider &dynamicCollider, const Collider &staticCollider);            // Resolves a dynamic body against a static collider
    static void ResolveAABB(Transform &firstTransform, Rigidbody &firstBody, Transform &secondTransform, Rigidbody &secondBody, const Collider &firstCollider, const Collider &secondCollider); // Resolves two dynamic bodies
};
