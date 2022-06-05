#ifndef _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include <GL/glew.h>


class VertexBuffer
{
    private:
        unsigned int m_RendererID; // unique ID returned by the graphic API library
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void bind() const;        // Locate the objecto into the OpenGL state Machine 
        void unbind() const;      // Unlocate the objecto into the OpenGL state Machine 
};


#endif