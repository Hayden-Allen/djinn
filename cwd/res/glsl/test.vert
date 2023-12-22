#version 450 core
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex;

uniform mat4 u_vp;
uniform vec3 u_pos;

out vec2 v_tex;

void main()
{
    gl_Position = u_vp * vec4(vec3(i_pos, 0) + u_pos * 25, 1);
    v_tex = i_tex;
}
