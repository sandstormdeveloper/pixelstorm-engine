#pragma once

class Registry;
class Entity;

class ParticleSystem
{
public:
    ParticleSystem();

    void Update(Registry &registry, float deltaTime); // Spawns, animates and destroys particles

private:
    float RandomRange(float minValue, float maxValue);                           // Returns a random value in the requested range
    void SpawnBurst(Registry &registry, const Entity &emitterEntity, int count); // Spawns a burst from one emitter

    unsigned int m_Seed; // Random seed used by the emitter
};
