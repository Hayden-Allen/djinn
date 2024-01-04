#version 410 core
TYPE(STATIC);
uniform mat4 d_vp;
out vec3 v_norm;

void main()
{
	mat4 transform = d_instance.d_model;
    gl_Position = d_vp * transform * vec4(d_pos, 1);
    v_norm = d_instance.d_normal * d_norm;
    // v_norm = d_norm;
}
