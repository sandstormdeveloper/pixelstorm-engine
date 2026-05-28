#include "pixelstorm/renderer/Texture.h"

#include "pixelstorm/core/Log.h"

#include <glad/glad.h>
#include "stb/stb_image.h"

namespace
{
    void ConfigureTexture2D(unsigned int textureId)
    {
        // Binds texture and configures sampler parameters
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void UploadTexturePixels(unsigned int textureId, int width, int height, const unsigned char *pixels)
    {
        // Uploads texture data to GPU and generates mipmaps
        ConfigureTexture2D(textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void UploadDefaultTexture(unsigned int textureId)
    {
        // Creates fallback 2x2 procedural texture
        const unsigned char pixels[] = {
            255, 255, 255, 255,
            255, 0, 0, 255,
            0, 255, 0, 255,
            0, 0, 255, 255};

        UploadTexturePixels(textureId, 2, 2, pixels);
    }
}

Texture::Texture()
    : m_ID(0)
{
    // Creates OpenGL texture
    glGenTextures(1, &m_ID);
    UploadDefaultTexture(m_ID);
    Log::Info("Default procedural texture created.");
}

Texture::Texture(const std::string &path)
    : m_ID(0)
{
    // Creates OpenGL texture
    glGenTextures(1, &m_ID);

    // Loads image pixels from file
    stbi_set_flip_vertically_on_load(0);

    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char *pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (pixels)
    {
        UploadTexturePixels(m_ID, width, height, pixels);
        stbi_image_free(pixels);
        Log::Info("Texture loaded from file: " + path);
        return;
    }

    Log::Warning("Failed to load texture from file. Falling back to default texture: " + path);

    // Falls back to procedural texture if loading fails
    UploadDefaultTexture(m_ID);
}

Texture::~Texture()
{
    // Frees texture resources
    if (m_ID != 0)
    {
        glDeleteTextures(1, &m_ID);
        Log::Info("Texture resources released.");
    }
}

void Texture::Bind(unsigned int slot) const
{
    // Activates texture slot
    glActiveTexture(GL_TEXTURE0 + slot);

    // Binds texture to slot
    glBindTexture(GL_TEXTURE_2D, m_ID);
}
