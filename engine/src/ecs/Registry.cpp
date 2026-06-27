#include "pixelstorm/ecs/Registry.h"

#include <algorithm>

Registry::Registry()
    : m_NextEntityId(1)
{
}

Entity Registry::CreateEntity(bool logCreation)
{
    // Creates a new entity and updates next ID
    const EntityId entityId = m_NextEntityId++;
    m_Entities.push_back(entityId);
    if (logCreation)
    {
        Log::Info("Entity created with id " + std::to_string(entityId) + ".");
    }
    return Entity(entityId, this);
}

Entity Registry::CreateEntity(const std::string &name)
{
    // Creates entity and stores a readable name for game/debug code
    Entity entity = CreateEntity(true);
    SetEntityName(entity, name);
    return entity;
}

Entity Registry::CreateEntitySilent(const std::string &name)
{
    // Creates entity without printing creation logs
    Entity entity = CreateEntity(false);
    m_EntityNames[entity.GetId()] = name;
    return entity;
}

void Registry::DestroyEntity(Entity entity, bool logDestruction)
{
    // Rejects invalid or already destroyed entities
    if (!HasEntity(entity))
    {
        Log::Warning("Attempted to destroy an invalid or missing entity.");
        return;
    }

    // Removes the entity from the active entity list so it disappears immediately from gameplay queries
    m_Entities.erase(
        std::remove(m_Entities.begin(), m_Entities.end(), entity.GetId()),
        m_Entities.end());

    // Removes the optional debug name
    m_EntityNames.erase(entity.GetId());

    // Defers component storage cleanup until the end of the frame
    m_DestroyedEntities.insert(entity.GetId());

    if (logDestruction)
    {
        Log::Info("Entity " + std::to_string(entity.GetId()) + " destroyed.");
    }
}

void Registry::FlushDestroyedEntities()
{
    // Nothing to do if no entities were marked for deferred cleanup
    if (m_DestroyedEntities.empty())
    {
        return;
    }

    // Frees all component storage for entities that were already removed from gameplay queries
    for (EntityId entityId : m_DestroyedEntities)
    {
        for (auto &entry : m_ComponentPools)
        {
            entry.second->RemoveEntity(entityId);
        }
    }

    m_DestroyedEntities.clear();
}

void Registry::SetEntityName(Entity entity, const std::string &name)
{
    // Sets entity name
    if (entity.IsValid())
    {
        m_EntityNames[entity.GetId()] = name;
        Log::Info("Entity " + std::to_string(entity.GetId()) + " named '" + name + "'.");
    }
    else
    {
        Log::Warning("Attempted to name an invalid entity.");
    }
}

std::string Registry::GetEntityName(Entity entity) const
{
    // Returns entity name
    std::unordered_map<EntityId, std::string>::const_iterator iterator = m_EntityNames.find(entity.GetId());
    if (iterator == m_EntityNames.end())
    {
        return "";
    }

    return iterator->second;
}

bool Registry::HasEntity(Entity entity) const
{
    // Checks whether the registry still owns this entity id
    return entity.GetId() != 0 &&
           std::find(m_Entities.begin(), m_Entities.end(), entity.GetId()) != m_Entities.end();
}

void Registry::Clear()
{
    // Clears all scene-owned ECS state
    m_NextEntityId = 1;
    m_Entities.clear();
    m_DestroyedEntities.clear();
    m_EntityNames.clear();
    m_ComponentPools.clear();
    Log::Info("Registry cleared.");
}
