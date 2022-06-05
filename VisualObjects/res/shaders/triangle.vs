# version 330 core
layout (location = 0) in vec3 a_Position; // the position variable has attribute position 0
layout (location = 1) in vec3 a_Color; // the color variable has attribute position 1
layout (location = 2) in vec2 a_TexCoords; // the color variable has attribute position 1
  
out vec2 v_TexCoords;
uniform mat4 u_MVP; // model view and projection matrix

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0); // see how we directly give a vec3 to vec4's constructor
    // vertexColor = vec4(1.0, 0.0039, 0.9686, 1.0); // set the output variable to a dark-red color
    // vertexColor = vec4(0.0471, 0.5137, 0.6549, 1.0); // set the output variable to a dark-red color
    v_TexCoords = a_TexCoords;
}