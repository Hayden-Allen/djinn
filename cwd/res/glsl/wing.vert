#version 410 core
TYPE(custom)
layout(location = 0) in vec4 i_pos;
uniform mat4 d_vp;
void main()
{
    gl_Position = d_vp * d_instance.d_model * vec4(i_pos.xyz, 1);
}
