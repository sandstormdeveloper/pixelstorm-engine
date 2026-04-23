#version 330 core

// Inputs
in vec2 v_TexCoord;

// Outputs
out vec4 FragColor;

// Uniform texture
uniform sampler2D u_Texture;

// Uniform RGBA tint
uniform vec4 u_Color;

void main()
{
    // Final fragment color
    FragColor = texture(u_Texture, v_TexCoord) * u_Color;
}
