#version 330 core

// Inputs
in vec2 v_TexCoord;

// Outputs
out vec4 FragColor;

// Uniform texture
uniform sampler2D u_Texture;

void main()
{
    // Final fragment color
    FragColor = texture(u_Texture, v_TexCoord);
}
