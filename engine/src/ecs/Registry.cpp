#include "pixelstorm/ecs/Registry.h"

Registry::Registry()
    : m_NextEntityId(1)
{
}

Entity Registry::CreateEntity()
{
    // Creates a new entity and updates next ID
    const EntityId entityId = m_NextEntityId++;
    m_Entities.push_back(entityId);
    Log::Info("Entity created with id " + std::to_string(entityId) + ".");
    return Entity(entityId, this);
}

Entity Registry::CreateEntity(const std::string &name)
{
    // Creates entity and stores a readable name for game/debug code
    Entity entity = CreateEntity();
    SetEntityName(entity, name);
    return entity;
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

void Registry::Clear()
{
    // Clears all scene-owned ECS state
    m_NextEntityId = 1;
    m_Entities.clear();
    m_EntityNames.clear();
    m_ComponentPools.clear();
    Log::Info("Registry cleared.");
}
