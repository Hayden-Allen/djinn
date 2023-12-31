#version 450 core
TYPE(basic);
FIELD(vec4, color);
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex;
uniform mat4 d_vp;
out vec2 v_tex;
out vec4 v_color;

void main()
{
	mat4 transform = d_instance.d_transform;
    gl_Position = d_vp * transform * vec4(i_pos, 0, 1);
    v_tex = i_tex;
    v_color = d_instance.color;
}
