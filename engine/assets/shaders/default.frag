#version 330 core

// Inputs
in vec2 v_TexCoord;

// Outputs
out vec4 FragColor;

// Uniform texture
uniform sampler2D u_Texture;

// Selects between textured and flat-color rendering
uniform int u_UseTexture;

// Uniform RGBA tint
uniform vec4 u_Color;

// Normalized sub-region inside the texture: x, y, width, height
uniform vec4 u_SourceRect;

void main()
{
    // Final fragment color
    if (u_UseTexture == 1)
    {
        vec2 sourceUV = u_SourceRect.xy + v_TexCoord * u_SourceRect.zw;
        FragColor = texture(u_Texture, sourceUV) * u_Color;
    }
    else
    {
        FragColor = u_Color;
    }
}
