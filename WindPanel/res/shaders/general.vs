#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec3 a_TexCoord;
layout (location = 3) in vec3 a_TexIndex;

uniform mat4 u_Transform; // to translate the position

out vec4 v_Color;
out vec2 v_TexCoords;
out vec2 v_TexIndex;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    gl_Position = u_Transform * vec4(a_Position, 1.0);
}