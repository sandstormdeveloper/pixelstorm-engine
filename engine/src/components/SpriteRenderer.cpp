#include "pixelstorm/components/SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
    : TextureName(),
      Color(1.0f, 1.0f, 1.0f, 1.0f),
      Visible(true),
      RenderOrder(0),
      SourceRect(0, 0, 0, 0)
{
}

SpriteRenderer::SpriteRenderer(const glm::vec4 &color, bool visible)
    : TextureName(),
      Color(color),
      Visible(visible),
      RenderOrder(0),
      SourceRect(0, 0, 0, 0)
{
}

SpriteRenderer::SpriteRenderer(const std::string &textureName, const glm::vec4 &color, bool visible, int renderOrder)
    : TextureName(textureName),
      Color(color),
      Visible(visible),
      RenderOrder(renderOrder),
      SourceRect(0, 0, 0, 0)
{
}
