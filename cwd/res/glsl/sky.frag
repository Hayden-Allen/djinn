#version 410 core
layout(location = 0) out vec4 o_color;
in vec3 v_tc;
uniform samplerCube u_texture;
void main()
{
	o_color = texture(u_texture, v_tc);
}
