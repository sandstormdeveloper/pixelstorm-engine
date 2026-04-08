#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    void Bind(unsigned int slot = 0) const; // Activates and binds texture

private:
    unsigned int m_ID; // Texture ID
};
