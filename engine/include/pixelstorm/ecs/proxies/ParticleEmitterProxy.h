#pragma once

#include "pixelstorm/components/Particle.h"
#include "pixelstorm/core/Math.h"

#include <string>

class ParticleEmitterProxy
{
public:
    explicit ParticleEmitterProxy(ParticleEmitter &emitter)
        : m_Emitter(emitter)
    {
    }

    void Play()
    {
        // Enables automatic emission
        m_Emitter.Active = true;
    }

    void Pause()
    {
        // Disables automatic emission without clearing pending bursts
        m_Emitter.Active = false;
    }

    void Stop()
    {
        // Stops emission and clears queued burst requests
        m_Emitter.Active = false;
        m_Emitter.EmitAccumulator = 0.0f;
        m_Emitter.PendingBursts = 0;
    }

    bool IsPlaying() const
    {
        // Returns whether the emitter is currently active
        return m_Emitter.Active;
    }

    bool IsLooping() const
    {
        // Returns whether the emitter keeps auto emitting
        return m_Emitter.Loop;
    }

    void SetLoop(bool loop)
    {
        // Updates whether the emitter keeps auto emitting
        m_Emitter.Loop = loop;
    }

    bool IsAutoEmitting() const
    {
        // Returns whether the emitter spawns particles automatically
        return m_Emitter.AutoEmit;
    }

    void SetAutoEmit(bool autoEmit)
    {
        // Updates automatic emission state
        m_Emitter.AutoEmit = autoEmit;
    }

    bool IsOneShot() const
    {
        // Returns whether the emitter should self-destruct after spawning
        return m_Emitter.OneShot;
    }

    void SetOneShot(bool oneShot)
    {
        // Updates whether the emitter should self-destruct after spawning
        m_Emitter.OneShot = oneShot;
    }

    int GetBurstCount() const
    {
        // Returns how many particles are spawned by one burst
        return m_Emitter.BurstCount;
    }

    void SetBurstCount(int count)
    {
        // Updates the burst size used by gameplay code
        m_Emitter.BurstCount = count;
    }

    float GetEmitRate() const
    {
        // Returns the continuous emission rate
        return m_Emitter.EmitRate;
    }

    void SetEmitRate(float emitRate)
    {
        // Updates the continuous emission rate
        m_Emitter.EmitRate = emitRate;
    }

    float GetLifetime() const
    {
        // Returns the lifetime assigned to emitted particles
        return m_Emitter.Lifetime;
    }

    void SetLifetime(float lifetime)
    {
        // Updates the lifetime assigned to emitted particles
        m_Emitter.Lifetime = lifetime;
    }

    float GetSpeed() const
    {
        // Returns the base speed of emitted particles
        return m_Emitter.Speed;
    }

    void SetSpeed(float speed)
    {
        // Updates the base speed of emitted particles
        m_Emitter.Speed = speed;
    }

    float GetSpeedVariation() const
    {
        // Returns the random speed variation
        return m_Emitter.SpeedVariation;
    }

    void SetSpeedVariation(float variation)
    {
        // Updates the random speed variation
        m_Emitter.SpeedVariation = variation;
    }

    float GetSpread() const
    {
        // Returns the emission spread in degrees
        return m_Emitter.Spread;
    }

    void SetSpread(float spread)
    {
        // Updates the emission spread in degrees
        m_Emitter.Spread = spread;
    }

    Color GetStartColor() const
    {
        // Returns the initial particle color
        return m_Emitter.StartColor;
    }

    void SetStartColor(const Color &color)
    {
        // Updates the initial particle color
        m_Emitter.StartColor = color;
    }

    Color GetEndColor() const
    {
        // Returns the final particle color
        return m_Emitter.EndColor;
    }

    void SetEndColor(const Color &color)
    {
        // Updates the final particle color
        m_Emitter.EndColor = color;
    }

    Vec2 GetStartScale() const
    {
        // Returns the initial particle scale
        return m_Emitter.StartScale;
    }

    void SetStartScale(const Vec2 &scale)
    {
        // Updates the initial particle scale
        m_Emitter.StartScale = scale;
    }

    Vec2 GetEndScale() const
    {
        // Returns the final particle scale
        return m_Emitter.EndScale;
    }

    void SetEndScale(const Vec2 &scale)
    {
        // Updates the final particle scale
        m_Emitter.EndScale = scale;
    }

    float GetGravityScale() const
    {
        // Returns the gravity multiplier used by emitted particles
        return m_Emitter.GravityScale;
    }

    void SetGravityScale(float gravityScale)
    {
        // Updates the gravity multiplier used by emitted particles
        m_Emitter.GravityScale = gravityScale;
    }

    const std::string &GetTexture() const
    {
        // Returns the texture name used for emitted particles
        return m_Emitter.TextureName;
    }

    void SetTexture(const std::string &textureName)
    {
        // Updates the texture name used for emitted particles
        m_Emitter.TextureName = textureName;
    }

    int GetRenderOrder() const
    {
        // Returns the render order used by emitted particles
        return m_Emitter.RenderOrder;
    }

    void SetRenderOrder(int renderOrder)
    {
        // Updates the render order used by emitted particles
        m_Emitter.RenderOrder = renderOrder;
    }

    void EmitBurst(int count)
    {
        // Requests one or more bursts to be spawned by the particle system
        if (count > 0)
        {
            m_Emitter.PendingBursts += count;
        }
    }

private:
    ParticleEmitter &m_Emitter; // Particle emitter component used by the proxy
};
