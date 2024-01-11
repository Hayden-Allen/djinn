#version 410 core
TYPE(phorm);

uniform mat4 d_vp, d_model;
uniform mat3 d_normal;
out vec3 v_pos, v_norm;

void main()
{
    vec4 pos = vec4(3.75 * d_pos.x, 2.5 * d_pos.y, 3.75 * d_pos.z, 1);
    gl_Position = d_vp * d_model * pos;
    v_pos = vec3(d_model * pos);
    v_norm = d_normal * d_norm;
}