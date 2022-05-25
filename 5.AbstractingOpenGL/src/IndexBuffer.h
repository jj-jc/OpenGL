#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "Renderer.h"

class IndexBuffer
{
    private:
        unsigned int m_RendererID; // unique ID returned by the graphic API library
        unsigned int m_Count;      // number of elements inside the Index
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void bind() const;        // Locate the objecto into the OpenGL state Machine 
        void unbind() const;      // Unlocate the objecto into the OpenGL state Machine 

        inline unsigned int GetCount() const { return m_Count;}
};


#endif