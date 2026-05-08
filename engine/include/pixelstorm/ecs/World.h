#pragma once

#include "pixelstorm/ecs/Registry.h"

class World
{
public:
    World();
    explicit World(Registry &registry);

    Entity CreateEntity();                        // Creates a new entity with a unique ID
    Entity CreateEntity(const std::string &name); // Creates a named entity

    Registry &GetRegistry();             // Gets registry
    const Registry &GetRegistry() const; // Gets registry
    bool IsValid() const;                // Checks if registry is valid

private:
    Registry *m_Registry; // Registry used by world
};
