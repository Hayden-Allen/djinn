#version 410 core
TYPE(phorm);

uniform mat4 d_vp, d_model;
uniform mat3 d_normal;
out vec3 v_norm;

void main()
{
    gl_Position = d_vp * d_model * vec4(d_pos * .05, 1);
    v_norm = d_normal * d_norm;
}