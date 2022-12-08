#version 460 core

layout (location = 0) out vec4 FragColor;

uniform sampler3D texture_ID;
uniform float alpha;

in VS_OUT {
    vec3 tex_coord;
} vs_out;

void main()
{
    vec3 uvw = vs_out.tex_coord;
    vec3 col = texture(texture_ID, uvw).rrr;

    // if (length(col) < 0.1) discard;

    FragColor = vec4(col, alpha);
    // FragColor = vec4(uvw, 1.0);
}