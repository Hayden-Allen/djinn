#version 450 core
layout(location = 0) in vec3 i_pos;
layout(std140, binding = 0) uniform asdf
{
	mat4 m[256];
} u_instanced_transforms[12];
uniform mat4 u_vpr;

out vec3 v_tc;

void main()
{
	mat4 transform = u_instanced_transforms[0].m[gl_InstanceID];
	gl_Position = u_vpr * vec4(i_pos, 1);
	// gl_Position = vec4(i_pos, 1);
	v_tc = i_pos;
}