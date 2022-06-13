# version 330 core
layout (location = 0) in vec4 a_Vertex; // the position variable has attribute position 0
out vec2 v_TexCoords;

uniform mat4 u_MVP; // model view and projection matrix

void main()
{
    gl_Position = u_MVP * vec4(a_Vertex.xy, 0.0, 1.0); // see how we directly give a vec3 to vec4's constructor
    v_TexCoords = a_Vertex.zw;
}