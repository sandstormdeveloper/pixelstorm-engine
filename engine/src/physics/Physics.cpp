#include "pixelstorm/physics/Physics.h"
#include "pixelstorm/components/Collider.h"
#include "pixelstorm/components/Rigidbody.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/physics/Collision.h"

void Physics::Integrate(Transform &transform, const Rigidbody &rigidbody, float deltaTime)
{
    // Static bodies don't move through physics integration
    if (rigidbody.IsStatic)
    {
        return;
    }

    // Applies velocity to position using delta time
    transform.Position += rigidbody.Velocity * deltaTime;
}

void Physics::ResolveAABB(Transform &dynamicTransform, Rigidbody &dynamicBody, const Transform &staticTransform, const Collider &dynamicCollider, const Collider &staticCollider)
{
    // Static dynamic bodies should not be resolved as moving bodies
    if (dynamicBody.IsStatic)
    {
        return;
    }

    // Builds collision boxes
    AABB dynamicBox = Collision::CreateAABB(dynamicTransform, dynamicCollider);
    AABB staticBox = Collision::CreateAABB(staticTransform, staticCollider);

    // Gets collision details
    CollisionManifold manifold = Collision::CheckAABB(dynamicBox, staticBox);

    // Skips if there is no collision
    if (!manifold.IsColliding)
    {
        return;
    }

    // Triggers detect collision but don't block movement
    if (dynamicCollider.IsTrigger || staticCollider.IsTrigger)
    {
        return;
    }

    // Moves dynamic body out of the static collider
    dynamicTransform.Position += manifold.Normal * manifold.PenetrationDepth;

    // Stops velocity along the collision axis
    if (manifold.Normal.x != 0.0f)
    {
        dynamicBody.Velocity.x = 0.0f;
    }

    if (manifold.Normal.y != 0.0f)
    {
        dynamicBody.Velocity.y = 0.0f;
    }
}

void Physics::ResolveAABB(Transform &firstTransform, Rigidbody &firstBody, Transform &secondTransform, Rigidbody &secondBody, const Collider &firstCollider, const Collider &secondCollider)
{
    // Static bodies are handled by the dynamic-static resolver
    if (firstBody.IsStatic || secondBody.IsStatic)
    {
        return;
    }

    // Builds collision boxes
    AABB firstBox = Collision::CreateAABB(firstTransform, firstCollider);
    AABB secondBox = Collision::CreateAABB(secondTransform, secondCollider);

    // Gets collision details
    CollisionManifold manifold = Collision::CheckAABB(firstBox, secondBox);

    // Skips if there is no collision
    if (!manifold.IsColliding)
    {
        return;
    }

    // Triggers detect collision but don't block movement
    if (firstCollider.IsTrigger || secondCollider.IsTrigger)
    {
        return;
    }

    // Splits correction evenly between both dynamic bodies
    const glm::vec2 correction = manifold.Normal * (manifold.PenetrationDepth * 0.5f);
    firstTransform.Position += correction;
    secondTransform.Position -= correction;

    // Stops velocity along the collision axis for both bodies
    if (manifold.Normal.x != 0.0f)
    {
        firstBody.Velocity.x = 0.0f;
        secondBody.Velocity.x = 0.0f;
    }

    if (manifold.Normal.y != 0.0f)
    {
        firstBody.Velocity.y = 0.0f;
        secondBody.Velocity.y = 0.0f;
    }
}
