#version 450 core
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex;
layout(std140, binding = 0) uniform u_instanced_transforms
{
	mat4 m[256];
} u_transforms[12];
uniform mat4 u_vpr;

out vec2 v_tex;

void main()
{
	mat4 transform = u_transforms[0].m[gl_InstanceID];
    gl_Position = u_vpr * transform * vec4(i_pos, 0, 1);
    v_tex = i_tex;
}
