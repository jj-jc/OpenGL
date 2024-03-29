# version 330 
layout (location = 0) in vec3 position; // the position variable has attribute position 0
layout (location = 1) in vec3 color; // the color variable has attribute position 1
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(position, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(position, 1.0);
    // vertexColor = vec4(0.051, 0.3176, 0.8941, 1.0); // set the output variable to a dark-red color
    // vertexColor = vec4(0.2431, 0.9608, 0.0235, 1.0); // set the output variable to a dark-red color
}