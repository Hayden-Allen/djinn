#version 410 core
TYPE(phorm);

uniform mat4 d_vp, d_model;
uniform mat3 d_normal;
uniform float d_time;
out vec3 v_pos_world, v_norm_world;
out vec4 v_rgba, v_weights, v_uv[4];

void main()
{
    vec4 pos = vec4(d_pos, 1);
    pos.y += 2 * sin(d_time + gl_VertexID) * cos(d_time - gl_VertexID / 2);
    gl_Position = d_vp * d_model * pos;
    v_pos_world = vec3(d_model * pos);
    v_norm_world = d_normal * d_norm;
    v_rgba = d_rgba;
    v_weights = d_weights;
    v_uv[0] = d_uv0;
    v_uv[1] = d_uv1;
    v_uv[2] = d_uv2;
    v_uv[3] = d_uv3;
}