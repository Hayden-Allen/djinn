#version 410 core
layout(location = 0) out vec4 o_color;
in vec3 v_pos_world, v_norm_world;
uniform vec3 d_cam_pos;
in vec4 v_weights;

void main()
{
    vec3 N = normalize(v_norm_world);
    vec3 V = normalize(v_pos_world - d_cam_pos);
    LIGHT_SUM(N, V, v_pos_world);
    o_color = vec4(d_diffuse, 1);
}
