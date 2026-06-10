#pragma once

#include "pixelstorm/components/Rigidbody.h"
#include "pixelstorm/core/Math.h"

class RigidbodyProxy
{
public:
    explicit RigidbodyProxy(Rigidbody &rigidbody)
        : m_Rigidbody(rigidbody)
    {
    }

    Vec2 GetVelocity() const
    {
        // Returns rigidbody velocity
        return m_Rigidbody.Velocity;
    }

    void SetVelocity(const Vec2 &velocity)
    {
        // Updates rigidbody velocity
        m_Rigidbody.Velocity = velocity;
    }

    void AddVelocity(const Vec2 &delta)
    {
        // Adds velocity to rigidbody
        m_Rigidbody.Velocity += delta;
    }

    bool IsStatic() const
    {
        // Returns static state
        return m_Rigidbody.IsStatic;
    }

    void SetStatic(bool isStatic)
    {
        // Updates static state
        m_Rigidbody.IsStatic = isStatic;
    }

    bool UsesGravity() const
    {
        // Returns gravity state
        return m_Rigidbody.UseGravity;
    }

    void SetUseGravity(bool useGravity)
    {
        // Updates gravity state
        m_Rigidbody.UseGravity = useGravity;
    }

    float GetGravityScale() const
    {
        // Returns gravity multiplier
        return m_Rigidbody.GravityScale;
    }

    void SetGravityScale(float gravityScale)
    {
        // Updates gravity multiplier
        m_Rigidbody.GravityScale = gravityScale;
    }

private:
    Rigidbody &m_Rigidbody; // Rigidbody component
};
