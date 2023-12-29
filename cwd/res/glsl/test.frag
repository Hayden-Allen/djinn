#version 450 core
layout(location = 0) out vec4 o_color;
uniform sampler2D u_texture;
in vec2 v_tex;
in vec4 v_color;

void main()
{
    o_color = texture(u_texture, v_tex);
    // o_color = mix(texture(u_texture, v_tex), vec4(v_color.rgb, 1), v_color.a);
}
