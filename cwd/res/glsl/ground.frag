#version 410 core
layout(location = 0) out vec4 o_color;
uniform sampler2D u_texture;
in vec3 v_norm;
in vec2 v_tex;
void main()
{
    vec3 N = normalize(v_norm);
    vec3 L = normalize(vec3(-3, -2, -3));
    float NdL = max(0, dot(N, -L));
    vec4 tex = texture(u_texture, v_tex);
    o_color = vec4(min(tex.xyz * (vec3(NdL) + vec3(.2)), vec3(1)), 1);
}