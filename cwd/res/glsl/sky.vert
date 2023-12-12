#version 330 core
layout(location = 0) in vec3 i_pos;
out vec3 v_tc;
uniform mat4 u_vp;
void main()
{
	gl_Position = u_vp * vec4(i_pos, 1);
	v_tc = i_pos;
}