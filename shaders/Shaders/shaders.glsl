#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec4 vertexColor; // specify a color output to the fragment shader
uniform mat4 transform;

void main()
{
	gl_Position = position;
	vertexColor = vec4(color, 1.0); // set the output variable to a dark-red color
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

uniform vec4 colorTriangle;


void main()
{
	fragColor = colorTriangle;
};
