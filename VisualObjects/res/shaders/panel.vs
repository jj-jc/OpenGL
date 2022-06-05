# version 330 core
layout (location = 0) in vec3 a_Position; // the position variable has attribute position 0
  
uniform mat4 u_MVP = glm::mat4(1.0f); // model view and projection matrix

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0); // see how we directly give a vec3 to vec4's constructor
}