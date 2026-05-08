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

    float GetMass() const
    {
        // Returns rigidbody mass
        return m_Rigidbody.Mass;
    }

    void SetMass(float mass)
    {
        // Updates rigidbody mass
        m_Rigidbody.Mass = mass;
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

private:
    Rigidbody &m_Rigidbody; // Rigidbody component
};
