# version 330 core

in vec2 v_TexCoords;
out vec4 f_Color;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;
 
void main()
{
    // take red component for the bitmap texture (just one byte). Think as a color with the last component as the transparent.
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Text, v_TexCoords).r); 
    // the u_TextColo has the maximum transparent and the sampled depends on the bitmap.
    f_Color = vec4(u_TextColor, 1.0) * sampled;
} 
