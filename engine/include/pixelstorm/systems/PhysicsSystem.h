#pragma once

#include "pixelstorm/core/Math.h"

#include <cstdint>
#include <cstddef>
#include <unordered_set>
#include <vector>

class Registry;
class Entity;
struct Rigidbody;

using EntityId = std::uint32_t;

enum class TriggerEventType
{
    Enter,
    Stay,
    Exit
};

struct TriggerEvent
{
    TriggerEventType Type;
    EntityId First;
    EntityId Second;

    bool Involves(const Entity &entity) const; // Returns true if the event touches the entity
    EntityId Other(const Entity &entity) const; // Returns the other entity in the pair
};

class PhysicsSystem
{
public:
    PhysicsSystem();

    void Update(Registry &registry, float deltaTime); // Integrates movement, resolves collisions and gathers triggers

    void SetGravity(const Vec2 &gravity);                                   // Sets world gravity
    Vec2 GetGravity() const;                                                // Returns world gravity
    const std::vector<TriggerEvent> &GetTriggerEvents() const;              // Returns trigger events generated on the last frame
    std::vector<TriggerEvent> GetTriggerEventsFor(const Entity &entity) const; // Returns trigger events that involve one entity

private:
    struct TriggerPair
    {
        EntityId First;  // Smaller entity id in the pair
        EntityId Second; // Larger entity id in the pair

        bool operator==(const TriggerPair &other) const; // Compares normalized trigger pairs
    };

    struct TriggerPairHash
    {
        std::size_t operator()(const TriggerPair &pair) const; // Hashes a normalized trigger pair
    };

    static TriggerPair MakeTriggerPair(EntityId first, EntityId second); // Sorts ids so a pair is stable across frames

    void ApplyGravity(Rigidbody &rigidbody, float deltaTime); // Applies gravity to one body

    Vec2 m_Gravity;                                                          // Gravity acceleration used by platform-style bodies
    std::vector<TriggerEvent> m_TriggerEvents;                               // Trigger events collected during the current frame
    std::unordered_set<TriggerPair, TriggerPairHash> m_PreviousTriggerPairs; // Trigger overlaps active on the previous frame
};
