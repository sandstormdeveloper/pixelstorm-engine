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
    return Entity(entityId, this);
}
