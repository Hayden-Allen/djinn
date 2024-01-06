#version 410 core
TYPE(custom)
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_norm;
layout(location = 2) in vec2 i_tex;
uniform mat4 d_vp;
out vec3 v_norm;
out vec2 v_tex;
void main()
{
    gl_Position = d_vp * d_instance.d_model * vec4(i_pos, 1);
    v_norm = i_norm;
    v_tex = i_tex;
}