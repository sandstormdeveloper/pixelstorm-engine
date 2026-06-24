#pragma once

#include "pixelstorm/components/Animation.h"
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

    Entity CreateSprite(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName = "");                                                                                                            // Creates a visual entity
    Entity CreateAnimatedActor(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName, const AnimationClips &clips, const std::string &initialClip);                                             // Creates an animated actor entity with collider and rigidbody
    Entity CreateAnimatedSprite(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName, const glm::ivec2 &frameSize, int frameCount, int framesPerRow = 0, float fps = 12.0f, bool loop = true); // Creates a simple animated actor with one default clip
    Entity CreateStaticBox(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName = "", bool isTrigger = false);                                                                                 // Creates a static collider box
    Entity CreateActor(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName = "");                                                                                                             // Creates a dynamic actor box
    void DestroyEntity(Entity entity);                                                                                                                                                                                                                                      // Destroys an entity and all its components
    void Clear();                                                                                                                                                                                                                                                           // Removes all entities and components
    bool IsValid() const;                                                                                                                                                                                                                                                   // Checks if registry is valid

private:
    Registry &RequireRegistry();             // Gets registry or throws
    const Registry &RequireRegistry() const; // Gets registry or throws

    Registry *m_Registry; // Registry used by world
};
