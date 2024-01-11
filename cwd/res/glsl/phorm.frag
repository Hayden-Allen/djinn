#version 410 core
layout(location = 0) out vec4 o_color;
uniform vec3 d_cam_pos;
in vec3 v_pos, v_norm;

void main()
{
    vec3 N = normalize(v_norm);
    vec3 V = normalize(d_cam_pos - v_pos);
    LIGHT_SUM(N, V);
    o_color = vec4(min(d_ambient + d_diffuse + d_specular, vec3(1)), 1);
}
