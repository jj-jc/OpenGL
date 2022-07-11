#version 450 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32]; // this is the maximum OpenGL permits, but it is limited by the GPU too

void main()
{
    int index = int(v_TexIndex);
    o_Color = texture(u_Textures[index], v_TexCoord) * v_Color; // this is like a masck if there is no texture it has to value 1s.
}