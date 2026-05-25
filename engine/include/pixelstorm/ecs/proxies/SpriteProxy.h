#pragma once

#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/core/Math.h"

#include <string>

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

    std::string GetTexture() const
    {
        // Returns sprite texture resource name
        return m_Sprite.TextureName;
    }

    void SetTexture(const std::string &textureName)
    {
        // Updates sprite texture resource name
        m_Sprite.TextureName = textureName;
    }

    void ClearTexture()
    {
        // Removes the explicit sprite texture so the fallback texture is used
        m_Sprite.TextureName.clear();
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
