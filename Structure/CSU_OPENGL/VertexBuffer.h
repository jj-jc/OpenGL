#ifndef _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>


struct Vertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoords;
    float TexIndex;
};

class VertexBuffer
{
    private:
        unsigned int m_RendererID; // unique ID returned by the graphic API library
    public:
        VertexBuffer(const void* data, unsigned int size);
        VertexBuffer(const void* data, unsigned int size, GLenum usage);
        ~VertexBuffer();

        void bind() const;        // Locate the objecto into the OpenGL state Machine 
        void unbind() const;      // Unlocate the objecto into the OpenGL state Machine 

        unsigned int getRendererID() const;
};


#endif