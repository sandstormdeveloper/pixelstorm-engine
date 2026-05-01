#include "pixelstorm/ecs/Registry.h"

Registry::Registry()
    : m_NextEntityId(1)
{
}

Entity Registry::CreateEntity()
{
    // Creates a new entity
    return Entity(m_NextEntityId++, this);
}