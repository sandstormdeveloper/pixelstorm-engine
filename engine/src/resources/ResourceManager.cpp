#include "pixelstorm/resources/ResourceManager.h"
#include "pixelstorm/resources/AssetLoader.h"
#include "pixelstorm/renderer/Texture.h"

bool ResourceManager::LoadTexture(const std::string &name, const std::string &path)
{
    // Reuses texture if it already exists
    if (HasTexture(name))
    {
        return true;
    }

    // Loads texture from file
    std::unique_ptr<Texture> texture = AssetLoader::LoadTexture(path);
    if (!texture)
    {
        return false;
    }

    // Stores loaded texture by name
    m_Textures[name] = std::move(texture);
    return true;
}

bool ResourceManager::HasTexture(const std::string &name) const
{
    // Returns if texture exists in storage
    return m_Textures.find(name) != m_Textures.end();
}

Texture *ResourceManager::GetTexture(const std::string &name)
{
    // Gets texture iterator
    std::unordered_map<std::string, std::unique_ptr<Texture>>::iterator iterator = m_Textures.find(name);
    if (iterator == m_Textures.end())
    {
        return nullptr;
    }

    // Returns stored texture
    return iterator->second.get();
}

const Texture *ResourceManager::GetTexture(const std::string &name) const
{
    // Gets texture iterator in read-only mode
    std::unordered_map<std::string, std::unique_ptr<Texture>>::const_iterator iterator = m_Textures.find(name);
    if (iterator == m_Textures.end())
    {
        return nullptr;
    }

    // Returns stored texture
    return iterator->second.get();
}

void ResourceManager::Clear()
{
    // Clears stored textures
    m_Textures.clear();
}
