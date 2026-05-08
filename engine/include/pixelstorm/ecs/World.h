#pragma once

#include "pixelstorm/ecs/Entity.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <string>

class Registry;

class World
{
public:
    World();
    explicit World(Registry &registry);

    Entity CreateEntity();                        // Creates a new entity with a unique ID
    Entity CreateEntity(const std::string &name); // Creates a named entity

    Entity CreateSprite(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);                            // Creates a visual entity
    Entity CreateStaticBox(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, bool isTrigger = false); // Creates a static collider box
    Entity CreateActor(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);                             // Creates a dynamic actor box
    bool IsValid() const;                                                                                                                              // Checks if registry is valid

private:
    Registry &RequireRegistry();             // Gets registry or throws
    const Registry &RequireRegistry() const; // Gets registry or throws

    Registry *m_Registry; // Registry used by world
};
