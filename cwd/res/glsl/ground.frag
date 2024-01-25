#version 410 core
layout(location = 0) out vec4 o_color;
uniform sampler2D u_texture;
uniform float d_time;
uniform vec3 d_cam_pos;
in vec3 v_pos_world, v_norm_world;
in vec2 v_tex;
void main()
{
    // vec3 N = normalize(v_norm);
    // vec3 L = normalize(vec3(-3, -2, -3));
    // float NdL = max(0, dot(N, -L));
    // // vec4 tex = texture(u_texture, vec2(v_tex.x * 5, v_tex.y * 5 - fract(d_time)));
    // vec4 tex = texture(u_texture, vec2(v_tex.x * 5, v_tex.y * 5));
    // o_color = vec4(min(tex.xyz * (vec3(NdL) + vec3(.2)), vec3(1)), 1);

    vec3 N = normalize(v_norm_world);
    vec3 V = normalize(v_pos_world - d_cam_pos);
    LIGHT_SUM(N, V, v_pos_world);
    vec3 tex = texture(u_texture, vec2(v_tex.x * 5, v_tex.y * 5)).rgb;
    o_color = vec4(min(tex * (d_ambient + d_diffuse) + d_specular, vec3(1)), 1);
}