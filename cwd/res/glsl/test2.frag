#version 450 core
layout(location = 0) out vec4 o_color;
uniform vec4 u_color1;
uniform vec4 u_color2;
uniform float u_colorMix;

void main()
{
    o_color = mix(u_color1, u_color2, .5 * u_colorMix);
}
