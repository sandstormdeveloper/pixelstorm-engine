#pragma once

#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/core/Math.h"

class SpriteProxy
{
public:
    explicit SpriteProxy(SpriteRenderer &sprite)
        : m_Sprite(sprite)
    {
    }

    Color GetColor() const
    {
        // Returns sprite color
        return m_Sprite.Color;
    }

    void SetColor(const Color &color)
    {
        // Updates sprite color
        m_Sprite.Color = color;
    }

    bool IsVisible() const
    {
        // Returns sprite visibility
        return m_Sprite.Visible;
    }

    void SetVisible(bool visible)
    {
        // Updates sprite visibility
        m_Sprite.Visible = visible;
    }

    void Show()
    {
        // Makes sprite visible
        m_Sprite.Visible = true;
    }

    void Hide()
    {
        // Makes sprite hidden
        m_Sprite.Visible = false;
    }

private:
    SpriteRenderer &m_Sprite; // Sprite component
};
