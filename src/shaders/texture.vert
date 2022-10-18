#version 460 core

layout (location = 0) in vec2 a_pos;

out VS_OUT {
    vec2 tex_coord;
} vs_out;

void main()
{
    vs_out.tex_coord = a_pos*0.5+9.5;
    gl_Position = vec4(a_pos, 0.0, 1.0);
}