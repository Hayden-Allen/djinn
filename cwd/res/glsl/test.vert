#version 450 core
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex;
/*
struct instance
{
    mat4 m;
    vec4 v;
};
layout(std140, binding = 0) uniform u_instanced_transforms
{
	instance i[128];
} u_transforms[12];
*/
layout(std140, binding = 0) uniform u_instanced_transforms
{
	mat4 m[256];
} u_transforms[12];

uniform mat4 u_vpr;

out vec2 v_tex;

void main()
{
    int blockIndex = gl_InstanceID / 256;
    int transformIndex = gl_InstanceID - 256 * blockIndex;
	mat4 transform = u_transforms[blockIndex].m[transformIndex];
    gl_Position = u_vpr * transform * vec4(i_pos, 0, 1);
    v_tex = i_tex;
}
