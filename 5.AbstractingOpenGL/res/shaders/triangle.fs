# version 330
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

uniform vec4 a_Color = vec4(0.2627, 0.8706, 0.1098, 1.0);
// uniform vec4 a_Color = vertexColor;

void main()
{
    // a_Color = vertexColor;
    FragColor = a_Color;
} 
