#version 410 core
TYPE(basic);
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_norm;
layout(location = 2) in vec2 i_tex;
uniform mat4 d_vp;
out vec3 v_norm;

void main()
{
	// mat4 transform = d_instance.d_transform;
    gl_Position = d_vp * vec4(vec3(i_pos.x, i_pos.y, i_pos.z - 5), 1);
    v_norm = i_norm;
}
