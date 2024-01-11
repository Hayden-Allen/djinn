#version 410 core
TYPE(phorm);

uniform mat4 d_vp, d_model;
uniform mat3 d_normal;
out vec3 v_pos, v_norm;

void main()
{
    vec4 pos = vec4(d_pos * .05, 1);
    gl_Position = d_vp * d_model * pos;
    v_pos = vec3(d_model * pos);
    v_norm = d_normal * d_norm;
}