#include "pixelstorm/renderer/Texture.h"
#include <glad/glad.h>

Texture::Texture()
    : m_ID(0)
{
    // 2x2 texture
    const unsigned char pixels[] = {
        255, 255, 255, 255,
        255, 0, 0, 255,
        0, 255, 0, 255,
        0, 0, 255, 255};

    // Creates OpenGL texture
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // Defines texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Uploads texture to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Generates mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbinds texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    // Frees texture resources
    if (m_ID != 0)
    {
        glDeleteTextures(1, &m_ID);
    }
}

void Texture::Bind(unsigned int slot) const
{
    // Activates texture slot
    glActiveTexture(GL_TEXTURE0 + slot);

    // Binds texture to slot
    glBindTexture(GL_TEXTURE_2D, m_ID);
}
