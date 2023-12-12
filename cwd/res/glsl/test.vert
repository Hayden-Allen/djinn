#version 330 core
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec2 i_tc;
uniform mat4 u_mvp;
out vec2 v_tc;
void main()
{
	gl_Position = u_mvp * vec4(i_pos, 1);
	// gl_Position = vec4(i_pos.x, -i_pos.y, i_pos.z, 1);
	v_tc = i_tc;
}