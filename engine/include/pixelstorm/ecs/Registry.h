#pragma once

#include "pixelstorm/core/Log.h"
#include "pixelstorm/ecs/Entity.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

class IComponentPool
{
public:
    virtual ~IComponentPool() = default;
};

template <typename T>
class ComponentPool : public IComponentPool
{
public:
    std::unordered_map<EntityId, T> Components; // Component table
};

class Registry
{
public:
    Registry();

    Entity CreateEntity();                        // Creates a new entity with a unique ID
    Entity CreateEntity(const std::string &name); // Creates a named entity

    template <typename T, typename... Args>
    T &AddComponent(Entity entity, Args &&...args); // Adds component to entity

    template <typename T>
    bool HasComponent(Entity entity) const; // Checks if entity has specific component

    template <typename T>
    T &GetComponent(Entity entity); // Returns specific component

    template <typename T>
    const T &GetComponent(Entity entity) const; // Returns specific component in read-only mode

    template <typename T>
    void RemoveComponent(Entity entity); // Removes component from entity

    template <typename... Components>
    std::vector<Entity> GetEntitiesWith(); // Returns entities with specific components

    void SetEntityName(Entity entity, const std::string &name); // Stores a debug/game-facing entity name
    std::string GetEntityName(Entity entity) const;             // Returns entity name, if any
    void Clear();                                               // Removes all entities and components

private:
    template <typename T>
    ComponentPool<T> *GetComponentPool(); // Returns component pool if it exists

    template <typename T>
    const ComponentPool<T> *GetComponentPool() const; // Returns component pool if it exists

    template <typename T>
    ComponentPool<T> *GetOrCreateComponentPool(); // Returns component pool (creates it if it doesn't exist)

    EntityId m_NextEntityId;                                                               // Next entity ID
    std::vector<EntityId> m_Entities;                                                      // Created entities
    std::unordered_map<EntityId, std::string> m_EntityNames;                               // Optional entity names
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_ComponentPools; // All component pools
};

template <typename T, typename... Args>
T &Registry::AddComponent(Entity entity, Args &&...args)
{
    // Gets or creates the pool for this component type
    ComponentPool<T> *pool = GetOrCreateComponentPool<T>();

    // Inserts the component for the entity if it doesn't exist yet
    typename std::unordered_map<EntityId, T>::iterator iterator = pool->Components.find(entity.GetId());
    if (iterator == pool->Components.end())
    {
        iterator = pool->Components.emplace(entity.GetId(), T(std::forward<Args>(args)...)).first;
    }
    else
    {
        // Warns if a component is added twice to the same entity
        Log::Warning("Entity " + std::to_string(entity.GetId()) + " already has this component. Existing instance will be reused.");
    }

    return iterator->second;
}

template <typename T>
bool Registry::HasComponent(Entity entity) const
{
    // Gets the pool if it already exists
    const ComponentPool<T> *pool = GetComponentPool<T>();
    if (!pool)
    {
        return false;
    }

    // Checks if the entity ID exists in the pool
    typename std::unordered_map<EntityId, T>::const_iterator iterator = pool->Components.find(entity.GetId());
    return iterator != pool->Components.end();
}

template <typename T>
T &Registry::GetComponent(Entity entity)
{
    // Gets the component pool for this type
    ComponentPool<T> *pool = GetComponentPool<T>();
    if (!pool)
    {
        throw std::runtime_error("Component pool not found.");
    }

    // Gets the component stored for the entity
    typename std::unordered_map<EntityId, T>::iterator iterator = pool->Components.find(entity.GetId());
    if (iterator == pool->Components.end())
    {
        throw std::runtime_error("Component not found for entity.");
    }

    return iterator->second;
}

template <typename T>
const T &Registry::GetComponent(Entity entity) const
{
    // Gets the component pool for this type in read-only mode
    const ComponentPool<T> *pool = GetComponentPool<T>();
    if (!pool)
    {
        throw std::runtime_error("Component pool not found.");
    }

    // Gets the component stored for the entity
    typename std::unordered_map<EntityId, T>::const_iterator iterator = pool->Components.find(entity.GetId());
    if (iterator == pool->Components.end())
    {
        throw std::runtime_error("Component not found for entity.");
    }

    return iterator->second;
}

template <typename T>
void Registry::RemoveComponent(Entity entity)
{
    // Gets the pool if it exists and removes the component entry
    ComponentPool<T> *pool = GetComponentPool<T>();
    if (pool)
    {
        pool->Components.erase(entity.GetId());
    }
    else
    {
        // Warns if there is no pool for the component type
        Log::Warning("Attempted to remove a component from entity " + std::to_string(entity.GetId()) + ", but no pool exists for that component type.");
    }
}

template <typename T>
ComponentPool<T> *Registry::GetComponentPool()
{
    // Looks up the component pool by type
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>>::iterator iterator = m_ComponentPools.find(std::type_index(typeid(T)));
    if (iterator == m_ComponentPools.end())
    {
        return nullptr;
    }

    return static_cast<ComponentPool<T> *>(iterator->second.get());
}

template <typename T>
const ComponentPool<T> *Registry::GetComponentPool() const
{
    // Looks up the component pool by type in read-only context
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>>::const_iterator iterator = m_ComponentPools.find(std::type_index(typeid(T)));
    if (iterator == m_ComponentPools.end())
    {
        return nullptr;
    }

    return static_cast<const ComponentPool<T> *>(iterator->second.get());
}

template <typename T>
ComponentPool<T> *Registry::GetOrCreateComponentPool()
{
    // Returns the pool if it already exists
    if (ComponentPool<T> *pool = GetComponentPool<T>())
    {
        return pool;
    }

    // Creates and stores a new pool for this component type
    std::unique_ptr<ComponentPool<T>> componentPool = std::make_unique<ComponentPool<T>>();
    ComponentPool<T> *pool = componentPool.get();
    m_ComponentPools[std::type_index(typeid(T))] = std::move(componentPool);
    return pool;
}

template <typename... Components>
std::vector<Entity> Registry::GetEntitiesWith()
{
    // Avoids function being called with 0 arguments
    static_assert(sizeof...(Components) > 0, "GetEntitiesWith requires at least one component type.");

    // List of entities with specified components
    std::vector<Entity> result;

    // Loops through entities
    for (EntityId entityId : m_Entities)
    {
        Entity entity(entityId, this);

        // Checks if entity has all components
        if ((HasComponent<Components>(entity) && ...))
        {
            // Adds entity to returned list
            result.push_back(entity);
        }
    }

    return result;
}
