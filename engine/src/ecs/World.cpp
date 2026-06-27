#include "pixelstorm/ecs/World.h"

#include "pixelstorm/components/Animation.h"
#include "pixelstorm/components/Collider.h"
#include "pixelstorm/components/Particle.h"
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

Entity World::CreateSprite(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName)
{
    // Creates named entity
    Entity entity = RequireRegistry().CreateEntitySilent(name);

    // Adds transform and visual component
    entity.AddComponent<Transform>(position, size, 0.0f);
    entity.AddComponent<SpriteRenderer>(textureName, color);

    return entity;
}

Entity World::CreateAnimatedActor(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName, const AnimationClips &clips, const std::string &initialClip)
{
    // Creates a visible entity first so animation can be attached to a playable actor
    Entity entity = CreateSprite(name, position, size, color, textureName);

    // Adds collision and dynamic rigidbody so the animated entity can participate in gameplay
    entity.AddComponent<Collider>(size);
    entity.AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), false);

    // Adds an animator so the sprite can cycle through frames
    entity.AddComponent<Animator>();

    // Registers all the clips provided by the caller
    for (const auto &clipEntry : clips)
    {
        entity.Animation().AddClip(clipEntry.first, clipEntry.second);
    }

    // Picks the requested initial clip, falling back to the first available clip if needed
    if (!initialClip.empty() && entity.Animation().HasClip(initialClip))
    {
        entity.Animation().Play(initialClip);
    }
    else if (!clips.empty())
    {
        entity.Animation().Play(clips.begin()->first);
    }

    return entity;
}

Entity World::CreateAnimatedSprite(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName, const glm::ivec2 &frameSize, int frameCount, int framesPerRow, float fps, bool loop)
{
    // Builds a one-clip animation map for the legacy helper
    AnimationClips clips;
    clips.emplace("default", AnimationClip(frameSize, frameCount, framesPerRow, fps, loop, 0));

    // Reuses the newer animated actor helper so both APIs stay in sync
    return CreateAnimatedActor(name, position, size, color, textureName, clips, "default");
}

Entity World::CreateParticleEmitter(const std::string &name, const glm::vec2 &position, const std::string &textureName, int burstCount, float lifetime, float speed, float speedVariation, float spread, const glm::vec4 &startColor, const glm::vec4 &endColor, const glm::vec2 &startScale, const glm::vec2 &endScale, float gravityScale, int renderOrder, bool autoEmit, bool loop, float emitRate)
{
    // Creates a lightweight entity used only to drive particle effects
    Entity entity = CreateEntity(name);

    // Particles need a transform so the system knows where to spawn them
    entity.AddComponent<Transform>(position, glm::vec2(1.0f, 1.0f), 0.0f);

    // Builds the emitter configuration locally before storing it on the entity
    ParticleEmitter emitter;
    emitter.TextureName = textureName;
    emitter.BurstCount = burstCount;
    emitter.Lifetime = lifetime;
    emitter.Speed = speed;
    emitter.SpeedVariation = speedVariation;
    emitter.Spread = spread;
    emitter.StartColor = startColor;
    emitter.EndColor = endColor;
    emitter.StartScale = startScale;
    emitter.EndScale = endScale;
    emitter.GravityScale = gravityScale;
    emitter.RenderOrder = renderOrder;
    emitter.AutoEmit = autoEmit;
    emitter.Loop = loop;
    emitter.EmitRate = emitRate;
    emitter.Active = true;

    // Copies the configured emitter into the entity
    entity.AddComponent<ParticleEmitter>(emitter);

    return entity;
}

Entity World::CreateStaticBox(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName, bool isTrigger)
{
    // Creates visible entity first
    Entity entity = CreateSprite(name, position, size, color, textureName);

    // Adds collision and static rigidbody
    entity.AddComponent<Collider>(size, glm::vec2(0.0f, 0.0f), isTrigger);
    entity.AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), true);

    return entity;
}

Entity World::CreateActor(const std::string &name, const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const std::string &textureName)
{
    // Creates visible entity first
    Entity entity = CreateSprite(name, position, size, color, textureName);

    // Adds collision and dynamic rigidbody
    entity.AddComponent<Collider>(size);
    entity.AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), false);

    return entity;
}

void World::DestroyEntity(Entity entity)
{
    // Destroys the entity through the registry
    RequireRegistry().DestroyEntity(entity);
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
