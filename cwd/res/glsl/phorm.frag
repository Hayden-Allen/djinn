#version 410 core
layout(location = 0) out vec4 o_color;
uniform vec3 d_cam_pos;
in vec3 v_pos, v_norm;

void main()
{
    vec3 N = normalize(v_norm);
    vec3 V = normalize(d_cam_pos - v_pos);
    vec3 ambient = vec3(0), diffuse = vec3(0), specular = vec3(0);
    for(int i = 0; i < int(d_lights.d_num); i++)
    {
        vec3 L = normalize(d_lights.d_l[i].o2w[3].xyz);
        vec3 R = reflect(L, N);
        float NdL = max(0, dot(N, -L));
        float RdV = max(0, dot(R, V));
        ambient += d_lights.d_l[i].ca.rgb * d_lights.d_l[i].ca.a;
        diffuse += NdL * (d_lights.d_l[i].cd.rgb * d_lights.d_l[i].cd.a);
        specular += pow(RdV, 32) * (d_lights.d_l[i].cs.rgb * d_lights.d_l[i].cs.a);
    }
    o_color = vec4(min(ambient + diffuse + specular, vec3(1)), 1);
    // o_color = vec4(V, 1);
}
