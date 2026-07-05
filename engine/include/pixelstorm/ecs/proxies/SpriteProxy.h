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

    bool IsFlippedX() const
    {
        // Returns whether the sprite is flipped horizontally
        return m_Sprite.FlipX;
    }

    void SetFlippedX(bool flipped)
    {
        // Updates the horizontal flip state
        m_Sprite.FlipX = flipped;
    }

    void FlipX(bool flipped = true)
    {
        // Convenience helper for horizontal flips
        m_Sprite.FlipX = flipped;
    }

    bool IsFlippedY() const
    {
        // Returns whether the sprite is flipped vertically
        return m_Sprite.FlipY;
    }

    void SetFlippedY(bool flipped)
    {
        // Updates the vertical flip state
        m_Sprite.FlipY = flipped;
    }

    void FlipY(bool flipped = true)
    {
        // Convenience helper for vertical flips
        m_Sprite.FlipY = flipped;
    }

    void SetFlip(bool flippedX, bool flippedY)
    {
        // Updates both flip axes at once
        m_Sprite.FlipX = flippedX;
        m_Sprite.FlipY = flippedY;
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

    int GetRenderOrder() const
    {
        // Returns sprite render order
        return m_Sprite.RenderOrder;
    }

    void SetRenderOrder(int renderOrder)
    {
        // Updates sprite render order
        m_Sprite.RenderOrder = renderOrder;
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
