#version 450 core
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex;
struct instance
{
    mat4 d_transform;
    vec4 color;
};
layout(std140, binding = 0) uniform d_instance
{
	instance i[256];
} d_transforms[12];
uniform mat4 d_vp;
out vec2 v_tex;
out vec4 v_color;

void main()
{
    int blockIndex = gl_InstanceID / 256;
    int transformIndex = gl_InstanceID - 256 * blockIndex;
	mat4 transform = d_transforms[blockIndex].i[transformIndex].d_transform;
    gl_Position = d_vp * transform * vec4(i_pos, 0, 1);
    v_tex = i_tex;
    v_color = d_transforms[blockIndex].i[transformIndex].color;
}
