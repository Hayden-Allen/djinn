#version 410 core
layout(location = 0) out vec4 o_color;
uniform sampler2D u_texture_front, u_texture_back;
in vec3 v_pos, v_norm;
in vec2 v_uv;
in float v_tex_idx;
void main()
{
    vec3 N1 = normalize(v_norm);
    vec3 N2 = normalize(cross(dFdx(v_pos), dFdy(v_pos)));
    vec3 N = normalize(mix(N1, N2, .1));
    vec3 L = normalize(vec3(0, 0, 1));
    float NdL = max(0, dot(N, -L));

    if(v_tex_idx == 1)
        o_color = vec4(texture(u_texture_back, v_uv).rgb, 1);
    else
        o_color = vec4(texture(u_texture_front, v_uv).rgb, 1);
}