#version 410 core
TYPE(custom)
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_norm;
uniform mat4 d_vp;
out vec3 v_pos, v_norm;
void main()
{
    gl_Position = d_vp * d_instance.d_model * vec4(i_pos.xyz, 1);
    v_pos = gl_Position.xyz;
    v_norm = d_instance.d_normal * i_norm;
}
