#version 410 core
TYPE(custom)
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_norm;
layout(location = 2) in vec2 i_tex;
uniform mat4 d_vp;
out vec3 v_pos_world, v_norm_world;
out vec2 v_tex;
void main()
{
    vec4 pos = d_instance.d_model * vec4(i_pos, 1);
    gl_Position = d_vp * pos;
    v_pos_world = pos.xyz;
    v_norm_world = d_instance.d_normal * i_norm;
    v_tex = i_tex;
}