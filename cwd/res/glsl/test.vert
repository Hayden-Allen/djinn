#version 450 core
layout(location = 0) in vec2 i_pos;

void main()
{
    gl_Position = vec4(i_pos, 0, 1);
}
