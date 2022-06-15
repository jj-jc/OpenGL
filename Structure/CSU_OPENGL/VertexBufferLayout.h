#ifndef _VERTEX_BUFFER_LAYOUT_H
#define _VERTEX_BUFFER_LAYOUT_H

#include <GL/glew.h>

// #include "Renderer.h"
#include <vector>

struct VertexBufferElement // store the info needed to call glVertexAttribPointer
{
    unsigned int count;
    unsigned int type;
    bool normalize;
    // unsigned int size;
    // const void* offset;
    static unsigned int getSizeType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        return 0;
    }

};

class VertexBufferLayout
{
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;
    public:
        VertexBufferLayout():m_Stride(0){};

        template<typename T>
        void push(unsigned int count);

        inline const std::vector<VertexBufferElement>& getElements() const {return m_Elements;}
        inline unsigned int getStride() const {return m_Stride;}

};

#endif