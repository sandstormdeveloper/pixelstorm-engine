#pragma once

#include <memory>
#include <string>

class Texture;

class AssetLoader
{
public:
    static std::unique_ptr<Texture> LoadTexture(const std::string &path); // Loads texture from file path
};
