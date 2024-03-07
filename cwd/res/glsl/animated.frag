#version 410 core
layout(location = 0) out vec4 o_color;
uniform vec3 d_cam_pos;
uniform sampler2D u_texture;
in vec3 v_pos_world, v_norm_world;
in vec2 v_tex;
in vec4 v_weights;

void main()
{
    vec3 N = normalize(v_norm_world);
    vec3 V = normalize(v_pos_world - d_cam_pos);
    LIGHT_SUM(N, V, v_pos_world);
    vec3 tex_color = texture(u_texture, v_tex).rgb;
    o_color = vec4(tex_color * min(d_ambient + d_diffuse + d_specular, vec3(1)), 1);
}
