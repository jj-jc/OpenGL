#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//layout(location = 1) in vec3 color;
layout(location = 2) in vec2 aTexCoord;

out vec4 vertexColor; // specify a color output to the fragment shader
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
//	gl_Position = projection * view * model * position;
	gl_Position = model * position;
//	vertexColor = vec4(color, 1.0); // set the output variable to a dark-red color
	texCoord = aTexCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;
//in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)
in vec2 texCoord;  // the input variable from the vertex shader

uniform vec4 colorTriangle;
uniform float visibleLevel;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//fragColor = texture(ourTexture, texCoord) * vertexColor;	
	fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), visibleLevel);
	
};
