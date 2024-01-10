#version 410 core
TYPE(phorm);

uniform mat4 d_vp, d_model;
uniform mat3 d_normal;
out vec3 v_norm;

void main()
{
    vec3 pos = vec3(3.75 * d_pos.x, 2.5 * d_pos.y, 3.75 * d_pos.z);
    gl_Position = d_vp * d_model * vec4(pos, 1);
    v_norm = d_normal * d_norm;
}