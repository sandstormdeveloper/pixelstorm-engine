#include "pixelstorm/resources/AssetLoader.h"
#include "pixelstorm/renderer/Texture.h"

std::unique_ptr<Texture> AssetLoader::LoadTexture(const std::string &path) 
{
    return std::make_unique<Texture>(path);
}
