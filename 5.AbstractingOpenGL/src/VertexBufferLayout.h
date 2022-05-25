#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

// #include "Renderer.h"
#include <vector>
#include <GL/glew.h>

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

        // template<>
        // void push<float>(unsigned int count)
        // {
        //     m_Elements.push_back({count, GL_FLOAT, false});
        //     m_Stride += VertexBufferElement::getSizeType(GL_FLOAT);
        // }

        // template<>
        // void push<unsigned int>(unsigned int count)
        // {
        //     m_Elements.push_back({count, GL_UNSIGNED_INT, false});
        //     m_Stride += VertexBufferElement::getSizeType(GL_UNSIGNED_INT);
        // }

        // template<>
        // void push<unsigned char>(unsigned int count)
        // {
        //     m_Elements.push_back({count, GL_UNSIGNED_BYTE, true});
        //     m_Stride += VertexBufferElement::getSizeType(GL_UNSIGNED_BYTE);
        // }
        

// TODO: el std::vector<VertexBufferElement> como referencia en vez de una copia
        inline const std::vector<VertexBufferElement>& getElements() const {return m_Elements;}
        inline unsigned int getStride() const {return m_Stride;}

};


// template<typename T>
// void VertexBufferLayout::push(unsigned int count)
// {
//     static_assert(false);
// }




#endif