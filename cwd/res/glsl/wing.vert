#version 410 core
TYPE(custom)
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_norm;
layout(location = 2) in vec2 i_uv;
layout(location = 3) in float i_tex_idx;
uniform mat4 d_vp;
out vec3 v_pos, v_norm;
out vec2 v_uv;
out float v_tex_idx;
void main()
{
    // z-fighting 
    vec3 pos = vec3(i_pos.xy, i_pos.z - v_tex_idx * .01);
    gl_Position = d_vp * d_instance.d_model * vec4(pos, 1);
    v_pos = gl_Position.xyz;
    v_norm = d_instance.d_normal * i_norm;
    v_uv = i_uv;
    v_tex_idx = i_tex_idx;
}
