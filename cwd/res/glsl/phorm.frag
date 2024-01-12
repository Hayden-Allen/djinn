#version 410 core
layout(location = 0) out vec4 o_color;
uniform sampler2D d_phorm_tex0, d_phorm_tex1, d_phorm_tex2, d_phorm_tex3;
uniform vec3 d_cam_pos;
uniform float d_time;
in vec3 v_pos, v_norm;
in vec4 v_rgba, v_weights, v_uv[4];

void main()
{
    vec3 N = normalize(v_norm);
    vec3 V = normalize(d_cam_pos - v_pos);
    LIGHT_SUM(N, V);

	vec4 multi_tex_res =
        texture(d_phorm_tex0, v_uv[0].xy - v_uv[0].zw * fract(d_time)) * v_weights[0] +
        texture(d_phorm_tex1, v_uv[1].xy - v_uv[1].zw * fract(d_time)) * v_weights[1] +
        texture(d_phorm_tex2, v_uv[2].xy - v_uv[2].zw * fract(d_time)) * v_weights[2] +
        texture(d_phorm_tex3, v_uv[3].xy - v_uv[3].zw * fract(d_time)) * v_weights[3];
	vec3 mixed_res = mix(multi_tex_res.rgb, v_rgba.rgb, v_rgba.a);
    o_color = vec4(mixed_res * min(d_ambient + d_diffuse + d_specular, vec3(1)), 1);
}
