#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <string>

struct Particle
{
    glm::vec2 Velocity;   // Particle velocity in world space
    float Lifetime;       // Total time before the particle disappears
    float Age;            // Time already spent alive
    glm::vec4 StartColor; // Color when the particle is born
    glm::vec4 EndColor;   // Color when the particle is about to die
    glm::vec2 StartScale; // Size when the particle is born
    glm::vec2 EndScale;   // Size when the particle is about to die
    float GravityScale;   // Multiplier applied to the world gravity

    Particle();
    Particle(
        const glm::vec2 &velocity,
        float lifetime,
        const glm::vec4 &startColor,
        const glm::vec4 &endColor,
        const glm::vec2 &startScale,
        const glm::vec2 &endScale,
        float gravityScale);
};

struct ParticleEmitter
{
    bool Active;             // Whether the emitter is allowed to create particles
    bool Loop;               // Whether the emitter keeps spawning particles over time
    bool AutoEmit;           // Whether the emitter should spawn particles automatically
    int BurstCount;          // Number of particles created by one burst
    float EmitRate;          // Particles spawned per second while auto emitting
    float EmitAccumulator;   // Accumulated time for auto emission
    int PendingBursts;       // Number of bursts requested by gameplay code
    float Lifetime;          // Lifetime assigned to emitted particles
    float Speed;             // Base particle speed
    float SpeedVariation;    // Additional random speed variation
    float Spread;            // Angular spread in degrees
    glm::vec4 StartColor;    // Color at spawn time
    glm::vec4 EndColor;      // Color at the end of the particle life
    glm::vec2 StartScale;    // Initial particle scale
    glm::vec2 EndScale;      // Final particle scale
    float GravityScale;      // Gravity multiplier for emitted particles
    std::string TextureName; // Texture used by emitted particles
    int RenderOrder;         // Rendering order of emitted particles

    ParticleEmitter();
};
