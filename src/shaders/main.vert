#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_tex;

out VS_OUT {
    vec3 tex_coord;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    vs_out.tex_coord = a_tex;
}