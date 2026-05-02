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

    template <typename T, typename... Args>
    T &AddComponent(Args &&...args); // Adds component to entity

    template <typename T>
    bool HasComponent() const; // Checks if component exists

    template <typename T>
    T &GetComponent(); // Returns component

    template <typename T>
    void RemoveComponent(); // Removes component

private:
    Entity(EntityId id, Registry *registry); // Constructor used by registry

    EntityId m_Id;        // Entity ID
    Registry *m_Registry; // Registry

    friend class Registry; // Allows registry to access private constructor
};

template <typename T, typename... Args>
T &Entity::AddComponent(Args &&...args)
{
    return m_Registry->AddComponent<T>(*this, std::forward<Args>(args)...);
}

template <typename T>
bool Entity::HasComponent() const
{
    return m_Registry->HasComponent<T>(*this);
}

template <typename T>
T &Entity::GetComponent()
{
    return m_Registry->GetComponent<T>(*this);
}

template <typename T>
void Entity::RemoveComponent()
{
    m_Registry->RemoveComponent<T>(*this);
}
