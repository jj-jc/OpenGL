#include "VertexBufferLayout.h"

template<>
void VertexBufferLayout::push<float>(unsigned int count)
{
    m_Elements.push_back({count, GL_FLOAT, false});
    m_Stride += count * VertexBufferElement::getSizeType(GL_FLOAT);
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({count, GL_UNSIGNED_INT, false});
    m_Stride += count * VertexBufferElement::getSizeType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({count, GL_UNSIGNED_BYTE, true});
    m_Stride += count *VertexBufferElement::getSizeType(GL_UNSIGNED_BYTE);
}