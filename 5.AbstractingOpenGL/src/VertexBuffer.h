#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Renderer.h"

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