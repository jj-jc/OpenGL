# version 330 core

in vec2 v_TexCoords;
out vec4 f_Color;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;
 
void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Text, v_TexCoords).r);
    f_Color = vec4(u_TextColor, 1.0) * sampled;
} 
