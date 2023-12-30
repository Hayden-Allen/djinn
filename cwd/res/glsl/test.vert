#version 450 core
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex;
struct instance
{
    mat4 d_transform;
    vec4 color;
};
layout(std140, binding = 0) uniform u_instanced_transforms
{
	instance i[256];
} u_transforms[12];
uniform mat4 u_vp;
out vec2 v_tex;
out vec4 v_color;

void main()
{
    int blockIndex = gl_InstanceID / 256;
    int transformIndex = gl_InstanceID - 256 * blockIndex;
	mat4 transform = u_transforms[blockIndex].i[transformIndex].d_transform;
    gl_Position = u_vp * transform * vec4(i_pos, 0, 1);
    v_tex = i_tex;
    v_color = u_transforms[blockIndex].i[transformIndex].color;
}
