# version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoords;

uniform vec4 u_Color = vec4(0.2627, 0.8706, 0.1098, 1.0);
uniform sampler2D u_Texture; // to sample the texcoords
 
void main()
{
    // a_Color = vertexColor;
    // FragColor = u_Color;
    FragColor = texture(u_Texture, v_TexCoords);
    // FragColor = vec4(v_TexCoords, 0.0,1.0);
    

} 
