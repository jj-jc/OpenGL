#version 330 core

layout(location = 0) out vec4 fragColor;
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
	fragColor = vertexColor;
};