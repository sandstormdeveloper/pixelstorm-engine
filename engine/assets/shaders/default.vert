#version 330 core

// Inputs
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

// Outputs
out vec2 v_TexCoord;

// Uniform matrices
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    // Copies UVs
    v_TexCoord = aTexCoord;

    // Vertex position
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
}
