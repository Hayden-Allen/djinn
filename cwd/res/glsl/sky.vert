#version 410 core
TYPE(CUSTOM);
layout(location = 0) in vec3 i_pos;
uniform mat4 d_vpr;
out vec3 v_tc;

void main()
{
	mat4 transform = d_instance.d_model;
	gl_Position = d_vpr * transform * vec4(500 * i_pos, 1);
	v_tc = i_pos;
}
