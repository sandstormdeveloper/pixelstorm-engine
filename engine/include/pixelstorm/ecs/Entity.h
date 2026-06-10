#pragma once

#include "pixelstorm/core/Log.h"
#include "pixelstorm/core/Math.h"
#include "pixelstorm/ecs/proxies/ColliderProxy.h"
#include "pixelstorm/ecs/proxies/RigidbodyProxy.h"
#include "pixelstorm/ecs/proxies/SpriteProxy.h"
#include "pixelstorm/ecs/proxies/TriggerProxy.h"
#include "pixelstorm/ecs/proxies/TransformProxy.h"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

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
    const T &GetComponent() const; // Returns component in read-only mode

    template <typename T>
    void RemoveComponent(); // Removes component

    TransformProxy Transform(); // Returns transform helpers
    SpriteProxy Sprite();       // Returns sprite helpers
    ColliderProxy Collider();   // Returns collider helpers
    RigidbodyProxy Rigidbody(); // Returns rigidbody helpers
    TriggerProxy Trigger();     // Returns trigger helpers

private:
    template <typename T>
    T &RequireComponent(const char *helperName, const char *componentName); // Returns component or throws a clear error

    template <typename T>
    const T &RequireComponent(const char *helperName, const char *componentName) const; // Returns component or throws a clear error in read-only mode

    Entity(EntityId id, Registry *registry); // Constructor used by registry

    EntityId m_Id;        // Entity ID
    Registry *m_Registry; // Registry

    friend class Registry; // Allows registry to access private constructor
};

template <typename T, typename... Args>
T &Entity::AddComponent(Args &&...args)
{
    // Adds component to entity
    return m_Registry->AddComponent<T>(*this, std::forward<Args>(args)...);
}

template <typename T>
bool Entity::HasComponent() const
{
    // Checks if component exists
    return m_Registry->HasComponent<T>(*this);
}

template <typename T>
T &Entity::GetComponent()
{
    // Returns component
    return m_Registry->GetComponent<T>(*this);
}

template <typename T>
const T &Entity::GetComponent() const
{
    // Returns component in read-only mode
    return m_Registry->GetComponent<T>(*this);
}

template <typename T>
void Entity::RemoveComponent()
{
    // Removes component
    m_Registry->RemoveComponent<T>(*this);
}

template <typename T>
T &Entity::RequireComponent(const char *helperName, const char *componentName)
{
    // Returns component if it exists
    if (HasComponent<T>())
    {
        return GetComponent<T>();
    }

    // Fails with a helper-specific message if the component is missing
    const std::string message = std::string("Entity::") + helperName + " requires " + componentName + " component.";
    Log::Error(message);
    throw std::runtime_error(message);
}

template <typename T>
const T &Entity::RequireComponent(const char *helperName, const char *componentName) const
{
    // Returns component if it exists
    if (HasComponent<T>())
    {
        return GetComponent<T>();
    }

    // Fails with a helper-specific message if the component is missing
    const std::string message = std::string("Entity::") + helperName + " requires " + componentName + " component.";
    Log::Error(message);
    throw std::runtime_error(message);
}
