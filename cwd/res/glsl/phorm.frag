#version 410 core
layout(location = 0) out vec4 o_color;
in vec3 v_norm;

void main()
{
    vec3 N = normalize(v_norm);
    vec3 L = normalize(vec3(-3, -2, -3));
    float NdL = max(0, dot(N, -L));
    o_color = vec4(min(vec3(NdL) + vec3(.2), vec3(1)), 1);
}
