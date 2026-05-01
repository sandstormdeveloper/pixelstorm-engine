#include "pixelstorm/ecs/Entity.h"

Entity::Entity()
    : m_Id(0),
      m_Registry(nullptr)
{
}

EntityId Entity::GetId() const
{
    // Returns entity identifier
    return m_Id;
}

bool Entity::IsValid() const
{
    // Returns valid if entity was created by registry
    return (m_Id != 0 && m_Registry);
}

Entity::Entity(EntityId id, Registry *registry)
    : m_Id(id),
      m_Registry(registry)
{
}