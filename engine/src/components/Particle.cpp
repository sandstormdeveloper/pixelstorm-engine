#include "pixelstorm/components/Particle.h"

Particle::Particle()
    : Velocity(0.0f, 0.0f),
      Lifetime(1.0f),
      Age(0.0f),
      StartColor(1.0f, 1.0f, 1.0f, 1.0f),
      EndColor(1.0f, 1.0f, 1.0f, 0.0f),
      StartScale(4.0f, 4.0f),
      EndScale(1.0f, 1.0f),
      GravityScale(0.0f)
{
}

Particle::Particle(
    const glm::vec2 &velocity,
    float lifetime,
    const glm::vec4 &startColor,
    const glm::vec4 &endColor,
    const glm::vec2 &startScale,
    const glm::vec2 &endScale,
    float gravityScale)
    : Velocity(velocity),
      Lifetime(lifetime),
      Age(0.0f),
      StartColor(startColor),
      EndColor(endColor),
      StartScale(startScale),
      EndScale(endScale),
      GravityScale(gravityScale)
{
}

ParticleEmitter::ParticleEmitter()
    : Active(true),
      Loop(false),
      AutoEmit(false),
      BurstCount(8),
      EmitRate(0.0f),
      EmitAccumulator(0.0f),
      PendingBursts(0),
      Lifetime(0.75f),
      Speed(80.0f),
      SpeedVariation(20.0f),
      Spread(180.0f),
      StartColor(1.0f, 1.0f, 1.0f, 1.0f),
      EndColor(1.0f, 1.0f, 1.0f, 0.0f),
      StartScale(6.0f, 6.0f),
      EndScale(2.0f, 2.0f),
      GravityScale(0.0f),
      TextureName(),
      RenderOrder(0)
{
}
