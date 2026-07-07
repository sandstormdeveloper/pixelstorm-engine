#include "pixelstorm/systems/PhysicsSystem.h"

#include "pixelstorm/components/Collider.h"
#include "pixelstorm/components/Rigidbody.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/ecs/Entity.h"
#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/physics/Collision.h"
#include "pixelstorm/physics/Physics.h"

#include <cstddef>
#include <functional>
#include <utility>
#include <unordered_map>
#include <vector>

namespace
{
void DispatchTriggerCallback(Entity recipient, const Entity &other, TriggerEventType type)
{
    if (!recipient.IsValid() || !recipient.HasComponent<Collider>())
    {
        return;
    }

    // Routes the trigger event to the callback stored on the trigger collider
    Collider &collider = recipient.GetComponent<Collider>();
    if (!collider.IsTrigger)
    {
        return;
        }

        switch (type)
        {
        case TriggerEventType::Enter:
            if (collider.OnTriggerEnter)
            {
                collider.OnTriggerEnter(other);
            }
            break;
        case TriggerEventType::Stay:
            if (collider.OnTriggerStay)
            {
                collider.OnTriggerStay(other);
            }
            break;
        case TriggerEventType::Exit:
            if (collider.OnTriggerExit)
            {
                collider.OnTriggerExit(other);
            }
            break;
        }
    }
}

PhysicsSystem::PhysicsSystem()
    : m_Gravity(0.0f, 980.0f)
{
    // Starts with a default downward gravity
}

void PhysicsSystem::Update(Registry &registry, float deltaTime)
{
    // Clears the event buffer so the current frame starts fresh
    m_TriggerEvents.clear();

    // Gathers the movable physics bodies for this frame
    std::vector<Entity> physicsEntities = registry.GetEntitiesWith<Transform, Rigidbody, Collider>();

    // Integrates movement for all physics-enabled entities
    for (Entity entity : physicsEntities)
    {
        Rigidbody &rigidbody = entity.GetComponent<Rigidbody>();
        Transform &transform = entity.GetComponent<Transform>();

        ApplyGravity(rigidbody, deltaTime);
        Physics::Integrate(transform, rigidbody, deltaTime);
    }

    // Reuses collider-only entities for static and trigger checks
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

    // Collects trigger overlaps separately from physical resolution
    std::vector<Entity> triggerEntities = registry.GetEntitiesWith<Transform, Collider>();
    std::unordered_set<TriggerPair, TriggerPairHash> currentTriggerPairs;

    for (std::size_t firstIndex = 0; firstIndex < triggerEntities.size(); ++firstIndex)
    {
        Entity firstEntity = triggerEntities[firstIndex];
        const Collider &firstCollider = firstEntity.GetComponent<Collider>();

        for (std::size_t secondIndex = firstIndex + 1; secondIndex < triggerEntities.size(); ++secondIndex)
        {
            Entity secondEntity = triggerEntities[secondIndex];
            const Collider &secondCollider = secondEntity.GetComponent<Collider>();

            // Only one trigger is enough to treat the overlap as a sensor event
            if (!firstCollider.IsTrigger && !secondCollider.IsTrigger)
            {
                continue;
            }

            const Transform &firstTransform = firstEntity.GetComponent<Transform>();
            const Transform &secondTransform = secondEntity.GetComponent<Transform>();

            const AABB firstBox = Collision::CreateAABB(firstTransform, firstCollider);
            const AABB secondBox = Collision::CreateAABB(secondTransform, secondCollider);

            if (!Collision::Overlaps(firstBox, secondBox))
            {
                continue;
            }

            const TriggerPair pair = MakeTriggerPair(firstEntity.GetId(), secondEntity.GetId());
            currentTriggerPairs.insert(pair);

            // Emits Enter the first frame the overlap appears, Stay on later frames
            if (m_PreviousTriggerPairs.find(pair) == m_PreviousTriggerPairs.end())
            {
                m_TriggerEvents.push_back({TriggerEventType::Enter, pair.First, pair.Second});
            }
            else
            {
                m_TriggerEvents.push_back({TriggerEventType::Stay, pair.First, pair.Second});
            }
        }
    }

    for (const TriggerPair &pair : m_PreviousTriggerPairs)
    {
        // Emits Exit when an overlap disappears between frames
        if (currentTriggerPairs.find(pair) == currentTriggerPairs.end())
        {
            m_TriggerEvents.push_back({TriggerEventType::Exit, pair.First, pair.Second});
        }
    }

    // Builds a fast lookup so trigger callbacks can be dispatched by entity id
    std::unordered_map<EntityId, Entity> triggerLookup;
    for (const Entity &entity : triggerEntities)
    {
        triggerLookup.emplace(entity.GetId(), entity);
    }

    for (const TriggerEvent &event : m_TriggerEvents)
    {
        const EntityId firstId = event.First;
        const EntityId secondId = event.Second;

        const std::unordered_map<EntityId, Entity>::iterator firstIt = triggerLookup.find(firstId);
        const std::unordered_map<EntityId, Entity>::iterator secondIt = triggerLookup.find(secondId);

        if (firstIt != triggerLookup.end() && firstIt->second.IsValid())
        {
            DispatchTriggerCallback(firstIt->second, secondIt != triggerLookup.end() ? secondIt->second : firstIt->second, event.Type);
        }

        if (secondIt != triggerLookup.end() && secondIt->second.IsValid())
        {
            DispatchTriggerCallback(secondIt->second, firstIt != triggerLookup.end() ? firstIt->second : secondIt->second, event.Type);
        }
    }

    m_PreviousTriggerPairs = std::move(currentTriggerPairs);
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

const std::vector<TriggerEvent> &PhysicsSystem::GetTriggerEvents() const
{
    return m_TriggerEvents;
}

std::vector<TriggerEvent> PhysicsSystem::GetTriggerEventsFor(const Entity &entity) const
{
    // Filters the current event buffer to only the events that involve one entity
    std::vector<TriggerEvent> filteredEvents;
    for (const TriggerEvent &event : m_TriggerEvents)
    {
        if (event.Involves(entity))
        {
            filteredEvents.push_back(event);
        }
    }

    return filteredEvents;
}

bool TriggerEvent::Involves(const Entity &entity) const
{
    // Checks whether the event belongs to the requested entity
    return First == entity.GetId() || Second == entity.GetId();
}

EntityId TriggerEvent::Other(const Entity &entity) const
{
    // Returns the opposite entity id from the trigger pair
    if (First == entity.GetId())
    {
        return Second;
    }

    return First;
}

bool PhysicsSystem::TriggerPair::operator==(const TriggerPair &other) const
{
    // Normalized pairs compare by id order and content
    return First == other.First && Second == other.Second;
}

std::size_t PhysicsSystem::TriggerPairHash::operator()(const TriggerPair &pair) const
{
    // Combines both entity ids into a stable hash key
    const std::size_t firstHash = std::hash<EntityId>{}(pair.First);
    const std::size_t secondHash = std::hash<EntityId>{}(pair.Second);
    return firstHash ^ (secondHash << 1);
}

PhysicsSystem::TriggerPair PhysicsSystem::MakeTriggerPair(EntityId first, EntityId second)
{
    // Sorts ids so the same two entities always produce the same pair
    if (first > second)
    {
        std::swap(first, second);
    }

    return TriggerPair{first, second};
}
