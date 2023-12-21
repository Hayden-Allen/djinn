#version 450 core
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex;

uniform mat4 u_vp;

out vec2 v_tex;

void main()
{
    gl_Position = u_vp * vec4(i_pos, 0, 1);
    v_tex = i_tex;
}
