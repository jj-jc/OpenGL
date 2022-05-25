#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
    private:
        unsigned int m_RendererID; // unique ID returned by the graphic API library
    public:
        VertexArray();
        ~VertexArray();

        void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

        void bind() const;        // Locate the objecto into the OpenGL state Machine 
        void unbind() const;      // Unlocate the objecto into the OpenGL state Machine 
        
};


#endif