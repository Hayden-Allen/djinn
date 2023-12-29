#version 450 core
layout(location = 0) in vec3 i_pos;
layout(std140, binding = 0) uniform u_instanced_transforms
{
	mat4 m[256];
} u_transforms[12];
uniform mat4 u_vpr;

out vec3 v_tc;

void main()
{
    int blockIndex = gl_InstanceID / 256;
    int transformIndex = gl_InstanceID - 256 * blockIndex;
	mat4 transform = u_transforms[blockIndex].m[transformIndex];
	gl_Position = u_vpr * transform * vec4(i_pos, 1);
	// gl_Position = vec4(i_pos, 1);
	v_tc = i_pos;
}