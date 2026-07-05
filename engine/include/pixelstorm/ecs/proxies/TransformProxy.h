#pragma once

#include "pixelstorm/components/Transform.h"
#include "pixelstorm/core/Math.h"

class TransformProxy
{
public:
    explicit TransformProxy(Transform &transform)
        : m_Transform(transform)
    {
    }

    Vec2 GetPosition() const
    {
        // Returns transform position
        return m_Transform.Position;
    }

    void SetPosition(const Vec2 &position)
    {
        // Updates transform position
        m_Transform.Position = position;
    }

    void Translate(const Vec2 &offset)
    {
        // Moves transform in world space
        m_Transform.Position += offset;
    }

    Vec2 GetScale() const
    {
        // Returns transform scale
        return m_Transform.Scale;
    }

    void SetScale(const Vec2 &scale)
    {
        // Updates transform scale
        m_Transform.Scale = scale;
    }

    Vec2 GetPivot() const
    {
        // Returns transform pivot
        return m_Transform.Pivot;
    }

    void SetPivot(const Vec2 &pivot)
    {
        // Updates transform pivot
        m_Transform.Pivot = pivot;
    }

    void TranslatePivot(const Vec2 &offset)
    {
        // Moves transform pivot in local space
        m_Transform.Pivot += offset;
    }

    float GetRotation() const
    {
        // Returns transform rotation
        return m_Transform.Rotation;
    }

    void SetRotation(float rotation)
    {
        // Updates transform rotation
        m_Transform.Rotation = rotation;
    }

    void Rotate(float deltaRotation)
    {
        // Rotates transform
        m_Transform.Rotation += deltaRotation;
    }

private:
    Transform &m_Transform; // Transform component
};
