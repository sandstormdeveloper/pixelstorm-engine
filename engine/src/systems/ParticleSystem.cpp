#include "pixelstorm/systems/ParticleSystem.h"

#include "pixelstorm/components/Particle.h"
#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/core/Log.h"
#include "pixelstorm/ecs/Entity.h"
#include "pixelstorm/ecs/Registry.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

namespace
{
    glm::vec4 LerpColor(const glm::vec4 &start, const glm::vec4 &end, float t)
    {
        return start + (end - start) * t;
    }

    glm::vec2 LerpVec2(const glm::vec2 &start, const glm::vec2 &end, float t)
    {
        return start + (end - start) * t;
    }
}

ParticleSystem::ParticleSystem()
    : m_Seed(std::random_device{}())
{
}

float ParticleSystem::RandomRange(float minValue, float maxValue)
{
    // Generates a pseudo-random value inside the requested interval
    m_Seed = m_Seed * 1664525u + 1013904223u;
    const float factor = static_cast<float>(m_Seed & 0x00FFFFFFu) / static_cast<float>(0x00FFFFFFu);
    return minValue + (maxValue - minValue) * factor;
}

void ParticleSystem::SpawnBurst(Registry &registry, const Entity &emitterEntity, int count)
{
    // Rejects invalid emitters before spawning particles
    if (!emitterEntity.IsValid() || count <= 0 || !emitterEntity.HasComponent<ParticleEmitter>() || !emitterEntity.HasComponent<Transform>())
    {
        return;
    }

    const ParticleEmitter &emitter = emitterEntity.GetComponent<ParticleEmitter>();
    const Transform &emitterTransform = emitterEntity.GetComponent<Transform>();

    // Spawns a small cloud of particle entities around the emitter
    for (int index = 0; index < count; ++index)
    {
        const float angleDegrees = RandomRange(-emitter.Spread * 0.5f, emitter.Spread * 0.5f);
        const float angleRadians = angleDegrees * 3.14159265f / 180.0f;
        const float speed = std::max(0.0f, emitter.Speed + RandomRange(-emitter.SpeedVariation, emitter.SpeedVariation));
        const glm::vec2 direction(std::cos(angleRadians), std::sin(angleRadians));
        const glm::vec2 velocity = direction * speed;

        Entity particleEntity = registry.CreateEntity(false);
        particleEntity.AddComponent<Transform>(emitterTransform.Position, emitter.StartScale, 0.0f);
        particleEntity.AddComponent<SpriteRenderer>(emitter.TextureName, emitter.StartColor, true, emitter.RenderOrder);
        particleEntity.AddComponent<Particle>(
            velocity,
            emitter.Lifetime,
            emitter.StartColor,
            emitter.EndColor,
            emitter.StartScale,
            emitter.EndScale,
            emitter.GravityScale);
    }
}

void ParticleSystem::Update(Registry &registry, float deltaTime)
{
    // Spawns particles from each active emitter before updating the living particles
    std::vector<Entity> emitters = registry.GetEntitiesWith<ParticleEmitter, Transform>();
    for (Entity emitterEntity : emitters)
    {
        ParticleEmitter &emitter = emitterEntity.GetComponent<ParticleEmitter>();
        if (!emitter.Active)
        {
            continue;
        }

        if (emitter.PendingBursts > 0)
        {
            SpawnBurst(registry, emitterEntity, emitter.PendingBursts);
            emitter.PendingBursts = 0;
        }

        if (emitter.AutoEmit && emitter.EmitRate > 0.0f)
        {
            emitter.EmitAccumulator += deltaTime;
            const float interval = 1.0f / emitter.EmitRate;
            while (emitter.EmitAccumulator >= interval)
            {
                emitter.EmitAccumulator -= interval;
                SpawnBurst(registry, emitterEntity, emitter.BurstCount);
            }
        }
    }

    // Updates all live particles and removes the ones that reached the end of their life
    std::vector<Entity> particles = registry.GetEntitiesWith<Particle, Transform, SpriteRenderer>();
    for (Entity particleEntity : particles)
    {
        Particle &particle = particleEntity.GetComponent<Particle>();
        Transform &transform = particleEntity.GetComponent<Transform>();
        SpriteRenderer &sprite = particleEntity.GetComponent<SpriteRenderer>();

        particle.Age += deltaTime;
        if (particle.Age >= particle.Lifetime)
        {
            particleEntity.Destroy(false);
            continue;
        }

        const float lifeT = std::clamp(particle.Age / particle.Lifetime, 0.0f, 1.0f);
        transform.Position += particle.Velocity * deltaTime;
        particle.Velocity += Vec2(0.0f, 980.0f * particle.GravityScale * deltaTime);
        transform.Scale = LerpVec2(particle.StartScale, particle.EndScale, lifeT);
        sprite.Color = LerpColor(particle.StartColor, particle.EndColor, lifeT);
    }
}
