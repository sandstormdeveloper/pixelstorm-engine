#pragma once

#include "pixelstorm/components/Collider.h"
#include "pixelstorm/core/Math.h"

class ColliderProxy
{
public:
    explicit ColliderProxy(Collider &collider)
        : m_Collider(collider)
    {
    }

    Vec2 GetSize() const
    {
        // Returns collider size
        return m_Collider.Size;
    }

    void SetSize(const Vec2 &size)
    {
        // Updates collider size
        m_Collider.Size = size;
    }

    Vec2 GetOffset() const
    {
        // Returns collider offset
        return m_Collider.Offset;
    }

    void SetOffset(const Vec2 &offset)
    {
        // Updates collider offset
        m_Collider.Offset = offset;
    }

    bool IsTrigger() const
    {
        // Returns trigger state
        return m_Collider.IsTrigger;
    }

    void SetTrigger(bool isTrigger)
    {
        // Updates trigger state
        m_Collider.IsTrigger = isTrigger;
    }

private:
    Collider &m_Collider; // Collider component
};
