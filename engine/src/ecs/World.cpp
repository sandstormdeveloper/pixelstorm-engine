#include "pixelstorm/ecs/World.h"

#include "pixelstorm/components/Collider.h"
#include "pixelstorm/components/Rigidbody.h"
#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/core/Log.h"
#include "pixelstorm/ecs/Registry.h"

#include <stdexcept>

World::World()
    : m_Registry(nullptr)
{
}

World::World(Registry &registry)
    : m_Registry(&registry)
{
}

Entity World::CreateEntity()
{
    // Creates entity with registry
    return RequireRegistry().CreateEntity();
}

Entity World::CreateEntity(const std::string &name)
{
    // Creates named entity with registry
    return RequireRegistry().CreateEntity(name);
}

Entity World::CreateSprite(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
{
    // Creates named entity
    Entity entity = CreateEntity(name);

    // Adds transform and visual component
    entity.AddComponent<Transform>(position, size, 0.0f);
    entity.AddComponent<SpriteRenderer>(color);

    return entity;
}

Entity World::CreateStaticBox(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, bool isTrigger)
{
    // Creates visible entity first
    Entity entity = CreateSprite(name, position, size, color);

    // Adds collision and static rigidbody
    entity.AddComponent<Collider>(size, glm::vec2(0.0f, 0.0f), isTrigger);
    entity.AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), true);

    return entity;
}

Entity World::CreateActor(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
{
    // Creates visible entity first
    Entity entity = CreateSprite(name, position, size, color);

    // Adds collision and dynamic rigidbody
    entity.AddComponent<Collider>(size);
    entity.AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), false);

    return entity;
}

void World::Clear()
{
    // Clears all entities and components from the registry
    RequireRegistry().Clear();
}

bool World::IsValid() const
{
    // Returns if world has a valid registry
    return m_Registry != nullptr;
}

Registry &World::RequireRegistry()
{
    // Gets registry if available
    if (!m_Registry)
    {
        Log::Error("World is not connected to a registry.");
        throw std::runtime_error("World is not connected to a registry.");
    }

    return *m_Registry;
}

const Registry &World::RequireRegistry() const
{
    // Gets registry if available in read-only mode
    if (!m_Registry)
    {
        Log::Error("World is not connected to a registry.");
        throw std::runtime_error("World is not connected to a registry.");
    }

    return *m_Registry;
}
