#version 410 core
layout(location = 0) out vec4 o_color;
in vec3 v_norm;
in vec4 v_weights;

void main()
{
    vec3 N = normalize(v_norm);
    vec3 L = normalize(vec3(0, -1, 0));
    float NdL = max(0, dot(N, -L));
    o_color = vec4(vec3(NdL), 1);
    o_color = vec4(N, 1);
    // o_color = vec4(v_weights);
}
