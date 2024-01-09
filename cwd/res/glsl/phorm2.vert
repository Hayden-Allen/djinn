#version 410 core
TYPE(custom);
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_norm;
layout(location = 2) in vec4 i_uv0;
layout(location = 3) in vec4 i_uv1;
layout(location = 4) in vec4 i_uv2;
layout(location = 5) in vec4 i_uv3;
layout(location = 6) in vec4 i_weights;
layout(location = 7) in vec4 i_rgba;

uniform mat4 d_vp;
out vec3 v_norm;

void main()
{
    vec3 pos = vec3(7.5 * i_pos.x, 5 * i_pos.y, 7.5 * i_pos.z);
    gl_Position = d_vp * vec4(pos, 1);
    v_norm = i_norm;
}