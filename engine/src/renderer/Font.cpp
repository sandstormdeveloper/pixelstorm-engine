#include "pixelstorm/renderer/Font.h"

#include "pixelstorm/core/Log.h"

#include <glad/glad.h>

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <vector>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FONT_BAKING
#define NK_IMPLEMENTATION
#include "nuklear.h"

namespace
{
    // Small realloc-based allocator used by the font baker
    void *NkAlloc(nk_handle, void *old, nk_size size)
    {
        if (size == 0)
        {
            std::free(old);
            return nullptr;
        }

        return std::realloc(old, static_cast<std::size_t>(size));
    }

    // Matching free callback for the font baker allocator
    void NkFree(nk_handle, void *old)
    {
        std::free(old);
    }

    // Reads the raw TTF bytes so Nuklear can bake the atlas from memory
    bool ReadBinaryFile(const std::string &path, std::vector<unsigned char> &bytes)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }

        bytes.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        return !bytes.empty();
    }

    // Configures the atlas texture with nearest sampling for crisp pixel text
    void ConfigureTexture2D(unsigned int textureId)
    {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

Font::Font(const std::string &path, float pixelHeight)
{
    Load(path, pixelHeight);
}

Font::~Font()
{
    Reset();
}

bool Font::Load(const std::string &path, float pixelHeight)
{
    Reset();

    std::vector<unsigned char> fontBytes;
    if (!ReadBinaryFile(path, fontBytes))
    {
        Log::Warning("Failed to read font file: " + path);
        return false;
    }

    nk_allocator permanent{};
    permanent.userdata.ptr = nullptr;
    permanent.alloc = NkAlloc;
    permanent.free = NkFree;

    nk_allocator temporary = permanent;

    nk_font_atlas atlas;
    // Builds the atlas in memory before uploading it to OpenGL
    nk_font_atlas_init_custom(&atlas, &permanent, &temporary);
    nk_font_atlas_begin(&atlas);

    struct nk_font_config config = nk_font_config(pixelHeight);
    nk_font *font = nk_font_atlas_add_from_memory(
        &atlas,
        fontBytes.data(),
        static_cast<nk_size>(fontBytes.size()),
        pixelHeight,
        &config);

    if (!font)
    {
        nk_font_atlas_cleanup(&atlas);
        Log::Warning("Failed to bake font atlas: " + path);
        return false;
    }

    int atlasWidth = 0;
    int atlasHeight = 0;
    // Bakes the atlas into an RGBA texture so it can be rendered like sprites
    const void *pixels = nk_font_atlas_bake(&atlas, &atlasWidth, &atlasHeight, NK_FONT_ATLAS_RGBA32);
    if (!pixels)
    {
        nk_font_atlas_cleanup(&atlas);
        Log::Warning("Failed to bake font pixels: " + path);
        return false;
    }

    glGenTextures(1, &m_TextureId);
    ConfigureTexture2D(m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_AtlasWidth = atlasWidth;
    m_AtlasHeight = atlasHeight;
    m_PixelHeight = pixelHeight;
    m_LineHeight = font->info.height;
    m_Ascent = font->info.ascent;
    m_Glyphs.clear();
    m_Glyphs.reserve(static_cast<std::size_t>(font->info.glyph_count));

    // Copies baked glyph metrics into a lookup table for fast drawing
    const nk_font_glyph *glyphs = atlas.glyphs + font->info.glyph_offset;
    for (int i = 0; i < font->info.glyph_count; ++i)
    {
        const nk_font_glyph &source = glyphs[i];
        Glyph glyph{};
        glyph.Codepoint = source.codepoint;
        glyph.XAdvance = source.xadvance;
        glyph.X0 = source.x0;
        glyph.Y0 = source.y0;
        glyph.X1 = source.x1;
        glyph.Y1 = source.y1;
        glyph.U0 = source.u0;
        glyph.V0 = source.v0;
        glyph.U1 = source.u1;
        glyph.V1 = source.v1;
        m_Glyphs[glyph.Codepoint] = glyph;
    }

    // Releases temporary baking memory after the atlas has been copied
    nk_font_atlas_cleanup(&atlas);
    Log::Info("Font loaded from file: " + path);
    return true;
}

void Font::Bind(unsigned int slot) const
{
    // Makes the atlas texture available to the active shader
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
}

bool Font::IsValid() const
{
    return m_TextureId != 0 && !m_Glyphs.empty();
}

float Font::GetPixelHeight() const
{
    return m_PixelHeight;
}

float Font::GetLineHeight() const
{
    return m_LineHeight;
}

float Font::GetAscent() const
{
    return m_Ascent;
}

const Font::Glyph *Font::FindGlyph(unsigned int codepoint) const
{
    // Looks up the glyph in the cached atlas table
    const auto iterator = m_Glyphs.find(codepoint);
    if (iterator == m_Glyphs.end())
    {
        return nullptr;
    }

    return &iterator->second;
}

void Font::Reset()
{
    // Frees the OpenGL texture if one was created
    if (m_TextureId != 0)
    {
        glDeleteTextures(1, &m_TextureId);
        m_TextureId = 0;
    }

    m_AtlasWidth = 0;
    m_AtlasHeight = 0;
    m_PixelHeight = 0.0f;
    m_LineHeight = 0.0f;
    m_Ascent = 0.0f;
    m_Glyphs.clear();
}
