#include "pixelstorm/components/SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
    : TextureName(),
      Color(1.0f, 1.0f, 1.0f, 1.0f),
      Visible(true)
{
}

SpriteRenderer::SpriteRenderer(const glm::vec4 &color, bool visible)
    : TextureName(),
      Color(color),
      Visible(visible)
{
}

SpriteRenderer::SpriteRenderer(const std::string &textureName, const glm::vec4 &color, bool visible)
    : TextureName(textureName),
      Color(color),
      Visible(visible)
{
}
