#version 460 core

layout (location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 tex_coord;
} vs_out;

void main()
{
    vec3 uvw = vs_out.tex_coord;
    FragColor = vec4(uvw, 1.0);
}