#version 450 core
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex;

out vec2 v_tex;

void main()
{
    gl_Position = vec4(i_pos, 0, 1);
    v_tex = i_tex;
}
