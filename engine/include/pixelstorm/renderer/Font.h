#pragma once

#include <string>
#include <unordered_map>

class Font
{
public:
    struct Glyph
    {
        unsigned int Codepoint; // Unicode codepoint
        float XAdvance;         // Cursor advance after drawing the glyph
        float X0;               // Local glyph bounds: left
        float Y0;               // Local glyph bounds: top
        float X1;               // Local glyph bounds: right
        float Y1;               // Local glyph bounds: bottom
        float U0;               // Atlas UV: left
        float V0;               // Atlas UV: top
        float U1;               // Atlas UV: right
        float V1;               // Atlas UV: bottom
    };

    Font() = default;
    Font(const std::string &path, float pixelHeight); // Loads a font from disk
    ~Font();                                          // Releases font resources

    bool Load(const std::string &path, float pixelHeight); // Loads or reloads the font
    void Bind(unsigned int slot = 0) const;                // Binds the font atlas texture
    bool IsValid() const;                                  // Returns if the font was loaded successfully
    float GetPixelHeight() const;                          // Returns the requested pixel height
    float GetLineHeight() const;                           // Returns the atlas line height
    float GetAscent() const;                               // Returns the ascent used to align the baseline
    const Glyph *FindGlyph(unsigned int codepoint) const;  // Returns glyph data for a codepoint

private:
    void Reset(); // Frees the current atlas and cached glyphs

    unsigned int m_TextureId = 0;                     // Font atlas texture ID
    int m_AtlasWidth = 0;                             // Atlas width in pixels
    int m_AtlasHeight = 0;                            // Atlas height in pixels
    float m_PixelHeight = 0.0f;                       // Requested font size
    float m_LineHeight = 0.0f;                        // Distance between lines
    float m_Ascent = 0.0f;                            // Baseline ascent in pixels
    std::unordered_map<unsigned int, Glyph> m_Glyphs; // Glyph cache keyed by codepoint
};
