#version 460 core

layout (location = 0) out vec4 FragColor;

in VS_OUT {
    vec2 tex_coord;
} vs_out;

void main()
{
    vec2 uv = vs_out.tex_coord;
    FragColor = vec4(uv, 0.0, 1.0);
}