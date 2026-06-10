#pragma once

#include "pixelstorm/components/Collider.h"
#include "pixelstorm/core/Math.h"

#include <functional>

class Entity;

class TriggerProxy
{
public:
    explicit TriggerProxy(Collider &collider)
        : m_Collider(collider)
    {
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

    void SetOnEnter(const std::function<void(Entity)> &callback)
    {
        // Registers enter callback
        m_Collider.OnTriggerEnter = callback;
    }

    void SetOnStay(const std::function<void(Entity)> &callback)
    {
        // Registers stay callback
        m_Collider.OnTriggerStay = callback;
    }

    void SetOnExit(const std::function<void(Entity)> &callback)
    {
        // Registers exit callback
        m_Collider.OnTriggerExit = callback;
    }

    void ClearCallbacks()
    {
        // Removes all trigger callbacks
        m_Collider.OnTriggerEnter = nullptr;
        m_Collider.OnTriggerStay = nullptr;
        m_Collider.OnTriggerExit = nullptr;
    }

private:
    Collider &m_Collider; // Collider component used as a trigger zone
};
