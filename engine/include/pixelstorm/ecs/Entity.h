#pragma once

#include <cstdint>

class Registry;

using EntityId = std::uint32_t;

class Entity
{
public:
    Entity();

    EntityId GetId() const; // Returns entity identifier
    bool IsValid() const;   // Confirms if entity was created by registry

private:
    Entity(EntityId id, Registry *registry); // Constructor used by registry

    EntityId m_Id;        // Entity ID
    Registry *m_Registry; // Registry

    friend class Registry; // Allows registry to access private constructor
};