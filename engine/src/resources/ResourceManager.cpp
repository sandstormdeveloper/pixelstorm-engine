#include "pixelstorm/resources/ResourceManager.h"
#include "pixelstorm/renderer/Texture.h"

bool ResourceManager::LoadTexture(const std::string &name, const std::string &path)
{
    (void)name;
    (void)path;
    return false;
}

bool ResourceManager::HasTexture(const std::string &name) const
{
    // Looks for texture in map
    return m_Textures.find(name) != m_Textures.end();
}

Texture *ResourceManager::GetTexture(const std::string &name)
{
    // Finds and returns specific texture
    std::unordered_map<std::string, std::unique_ptr<Texture>>::iterator iterator = m_Textures.find(name);
    if (iterator == m_Textures.end())
    {
        return nullptr;
    }

    return iterator->second.get();
}

const Texture *ResourceManager::GetTexture(const std::string &name) const
{
    // Finds and returns specific texture
    std::unordered_map<std::string, std::unique_ptr<Texture>>::const_iterator iterator = m_Textures.find(name);
    if (iterator == m_Textures.end())
    {
        return nullptr;
    }

    return iterator->second.get();
}

void ResourceManager::Clear()
{
    // Removes textures
    m_Textures.clear();
}
