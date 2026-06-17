#include "pixelstorm/ecs/Entity.h"

#include "pixelstorm/components/Collider.h"
#include "pixelstorm/components/Rigidbody.h"
#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/ecs/proxies/ColliderProxy.h"
#include "pixelstorm/ecs/proxies/RigidbodyProxy.h"
#include "pixelstorm/ecs/proxies/SpriteProxy.h"
#include "pixelstorm/ecs/proxies/TriggerProxy.h"
#include "pixelstorm/ecs/proxies/TransformProxy.h"

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
    // Returns valid if entity still exists in the registry
    return (m_Id != 0 && m_Registry && m_Registry->HasEntity(*this));
}

void Entity::Destroy()
{
    // Destroys this entity through the registry if possible
    if (m_Registry)
    {
        m_Registry->DestroyEntity(*this);
    }
}

Entity::Entity(EntityId id, Registry *registry)
    : m_Id(id),
      m_Registry(registry)
{
}

TransformProxy Entity::Transform()
{
    // Returns grouped transform helpers
    return TransformProxy(RequireComponent<::Transform>("Transform", "Transform"));
}

SpriteProxy Entity::Sprite()
{
    // Returns grouped sprite helpers
    return SpriteProxy(RequireComponent<::SpriteRenderer>("Sprite", "SpriteRenderer"));
}

ColliderProxy Entity::Collider()
{
    // Returns grouped collider helpers
    return ColliderProxy(RequireComponent<::Collider>("Collider", "Collider"));
}

RigidbodyProxy Entity::Rigidbody()
{
    // Returns grouped rigidbody helpers
    return RigidbodyProxy(RequireComponent<::Rigidbody>("Rigidbody", "Rigidbody"));
}

TriggerProxy Entity::Trigger()
{
    // Returns grouped trigger helpers
    return TriggerProxy(RequireComponent<::Collider>("Trigger", "Collider"));
}
