#version 460 core

layout (location = 0) out vec4 FragColor;

in VS_OUT {
    vec2 tex_coord;
} vs_out;

uniform sampler3D texture_ID;
uniform float depth;

void main()
{
    vec2 uv = vs_out.tex_coord;
    vec3 col = vec3(texture(texture_ID, vec3(uv, depth)).r);
    FragColor = vec4(col, 1.0);
}