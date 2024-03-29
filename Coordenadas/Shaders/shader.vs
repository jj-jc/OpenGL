#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
	gl_Position = position;
	vertexColor = vec4(color, 1.0); // set the output variable to a dark-red color
};