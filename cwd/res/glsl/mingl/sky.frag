#version 330 core
layout(location = 0) out vec4 o_color;
uniform samplerCube u_texture;
in vec3 v_tc;
void main()
{
	o_color = texture(u_texture, v_tc);
}