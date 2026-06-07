#include "pixelstorm/systems/PhysicsSystem.h"

#include "pixelstorm/components/Collider.h"
#include "pixelstorm/components/Rigidbody.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/ecs/Entity.h"
#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/physics/Physics.h"

#include <cstddef>
#include <vector>

PhysicsSystem::PhysicsSystem()
    : m_Gravity(0.0f, 980.0f)
{
}

void PhysicsSystem::Update(Registry &registry, float deltaTime)
{
    // Gets dynamic candidates with all physics components
    std::vector<Entity> physicsEntities = registry.GetEntitiesWith<Transform, Rigidbody, Collider>();

    // Integrates velocity into transform position
    for (Entity entity : physicsEntities)
    {
        Rigidbody &rigidbody = entity.GetComponent<Rigidbody>();
        Transform &transform = entity.GetComponent<Transform>();

        ApplyGravity(rigidbody, deltaTime);
        Physics::Integrate(transform, rigidbody, deltaTime);
    }

    // Gets every entity that can participate in collision checks
    std::vector<Entity> colliderEntities = registry.GetEntitiesWith<Transform, Collider>();

    // Resolves dynamic bodies against static colliders
    for (Entity dynamicEntity : physicsEntities)
    {
        Rigidbody &dynamicBody = dynamicEntity.GetComponent<Rigidbody>();
        if (dynamicBody.IsStatic)
        {
            continue;
        }

        Transform &dynamicTransform = dynamicEntity.GetComponent<Transform>();
        Collider &dynamicCollider = dynamicEntity.GetComponent<Collider>();

        for (Entity staticEntity : colliderEntities)
        {
            if (dynamicEntity.GetId() == staticEntity.GetId())
            {
                continue;
            }

            if (staticEntity.HasComponent<Rigidbody>() &&
                !staticEntity.GetComponent<Rigidbody>().IsStatic)
            {
                continue;
            }

            const Transform &staticTransform = staticEntity.GetComponent<Transform>();
            const Collider &staticCollider = staticEntity.GetComponent<Collider>();

            Physics::ResolveAABB(
                dynamicTransform,
                dynamicBody,
                staticTransform,
                dynamicCollider,
                staticCollider);
        }
    }

    // Resolves dynamic bodies against other dynamic bodies once per pair
    for (std::size_t firstIndex = 0; firstIndex < physicsEntities.size(); ++firstIndex)
    {
        Entity firstEntity = physicsEntities[firstIndex];
        Rigidbody &firstBody = firstEntity.GetComponent<Rigidbody>();
        if (firstBody.IsStatic)
        {
            continue;
        }

        Transform &firstTransform = firstEntity.GetComponent<Transform>();
        Collider &firstCollider = firstEntity.GetComponent<Collider>();

        for (std::size_t secondIndex = firstIndex + 1; secondIndex < physicsEntities.size(); ++secondIndex)
        {
            Entity secondEntity = physicsEntities[secondIndex];
            Rigidbody &secondBody = secondEntity.GetComponent<Rigidbody>();
            if (secondBody.IsStatic)
            {
                continue;
            }

            Transform &secondTransform = secondEntity.GetComponent<Transform>();
            Collider &secondCollider = secondEntity.GetComponent<Collider>();

            Physics::ResolveAABB(
                firstTransform,
                firstBody,
                secondTransform,
                secondBody,
                firstCollider,
                secondCollider);
        }
    }
}

void PhysicsSystem::SetGravity(const Vec2 &gravity)
{
    // Updates world gravity
    m_Gravity = gravity;
}

Vec2 PhysicsSystem::GetGravity() const
{
    // Returns world gravity
    return m_Gravity;
}

void PhysicsSystem::ApplyGravity(Rigidbody &rigidbody, float deltaTime)
{
    // Static bodies and top-down bodies ignore gravity
    if (rigidbody.IsStatic || !rigidbody.UseGravity)
    {
        return;
    }

    // Applies gravity acceleration to velocity
    rigidbody.Velocity += m_Gravity * rigidbody.GravityScale * deltaTime;
}
