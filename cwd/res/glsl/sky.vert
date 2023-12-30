#version 450 core
layout(location = 0) in vec3 i_pos;
struct instance
{
	mat4 d_transform;
};

layout(std140, binding = 0) uniform d_instance
{
	instance i[256];
} d_transforms[12];
uniform mat4 d_vpr;

out vec3 v_tc;

void main()
{
    int blockIndex = gl_InstanceID / 256;
    int transformIndex = gl_InstanceID - 256 * blockIndex;
	mat4 transform = d_transforms[blockIndex].i[transformIndex].d_transform;
	gl_Position = d_vpr * transform * vec4(i_pos, 1);
	v_tc = i_pos;
}