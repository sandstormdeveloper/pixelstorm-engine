#include "pixelstorm/ecs/World.h"

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
    return GetRegistry().CreateEntity();
}

Entity World::CreateEntity(const std::string &name)
{
    // Creates named entity with registry
    return GetRegistry().CreateEntity(name);
}

Registry &World::GetRegistry()
{
    // Gets registry if available
    if (!m_Registry)
    {
        throw std::runtime_error("World is not connected to a registry.");
    }

    return *m_Registry;
}

const Registry &World::GetRegistry() const
{
    // Gets registry if available
    if (!m_Registry)
    {
        throw std::runtime_error("World is not connected to a registry.");
    }

    return *m_Registry;
}

bool World::IsValid() const
{
    // Returns if world has a valid registry
    return m_Registry != nullptr;
}
