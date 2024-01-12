#version 410 core
TYPE(phorm);

uniform mat4 d_vp, d_model;
uniform mat3 d_normal;
out vec3 v_pos, v_norm;
out vec4 v_rgba, v_weights, v_uv0, v_uv1, v_uv2, v_uv3;

void main()
{
    vec4 pos = vec4(d_pos * .05, 1);
    gl_Position = d_vp * d_model * pos;
    v_pos = vec3(d_model * pos);
    v_norm = d_normal * d_norm;
    v_rgba = d_rgba;
    v_weights = d_weights;
    v_uv0 = d_uv0;
    v_uv1 = d_uv1;
    v_uv2 = d_uv2;
    v_uv3 = d_uv3;
}