#version 330 core

// Inputs
in vec2 v_TexCoord;

// Outputs
out vec4 FragColor;

// Scene rendered in the offscreen pass
uniform sampler2D u_Texture;

// Animated time used for scanlines and noise
uniform float u_Time;

float Hash(vec2 p)
{
    // Cheap hash for animated film grain
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

void main()
{
    // Warps the screen slightly to mimic a curved CRT panel
    vec2 centered = v_TexCoord * 2.0 - 1.0;
    float radius2 = dot(centered, centered);
    vec2 warpedUV = centered * (1.0 + radius2 * 0.045) * 0.5 + 0.5;

    // Adds a subtle horizontal wobble so the image feels more analog
    warpedUV.x += sin((warpedUV.y * 64.0) + u_Time * 2.0) * 0.0005;

    // Samples the scene with a small radial color split
    vec2 texel = 1.0 / vec2(textureSize(u_Texture, 0));
    vec2 chromaOffset = centered * texel * (1.5 + radius2 * 1.75);

    vec3 rgb;
    rgb.r = texture(u_Texture, warpedUV + chromaOffset).r;
    rgb.g = texture(u_Texture, warpedUV).g;
    rgb.b = texture(u_Texture, warpedUV - chromaOffset).b;

    // Softens the center slightly so bright elements feel a bit more luminous
    vec3 bloom =
        texture(u_Texture, warpedUV + vec2(texel.x, 0.0)).rgb +
        texture(u_Texture, warpedUV - vec2(texel.x, 0.0)).rgb +
        texture(u_Texture, warpedUV + vec2(0.0, texel.y)).rgb +
        texture(u_Texture, warpedUV - vec2(0.0, texel.y)).rgb;
    rgb = mix(rgb, bloom * 0.25, 0.12);

    // Emulates scanlines and a faint shadow mask
    float scanlines = 0.965 + 0.035 * sin(gl_FragCoord.y * 3.14159265);
    float mask = 0.985 + 0.015 * sin((gl_FragCoord.x + gl_FragCoord.y) * 0.5);
    rgb *= scanlines * mask;

    // Darkens the edges like a CRT bezel
    float vignette = 1.0 - smoothstep(0.45, 1.35, radius2);
    vignette = mix(1.0, vignette, 0.25);
    rgb *= vignette;

    // Adds animated grain on top of the image
    float grain = Hash(gl_FragCoord.xy + vec2(u_Time * 12.0, u_Time * 7.0)) - 0.5;
    rgb += grain * 0.015;

    FragColor = vec4(clamp(rgb, 0.0, 1.0), 1.0);
}
