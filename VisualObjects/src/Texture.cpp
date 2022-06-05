#include "Texture.h"


Texture::Texture(const std::string& path)
    :m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{  
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // OpenGL load textures with 0,0 in the left bottom, meanwhile in the jpg its 0,0 is on left top
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    if (m_LocalBuffer)
    {
        // there are two types of formats, internal format is how OpenGL stores the format
        // format of the texture data is how the data is stored (the differente channels and so one)
        // load the texture into GPU
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
        // glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        //FIXME:
        std::cout << "Failed to load texture" << std::endl;
    }

    // stbi_image_free(m_LocalBuffer); // to free the memory of the buffer
}
Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID); // free the texture data from the GPU
}

// OpenGL allows to bind more than 1 texture as the same time. The maximum depends on your platform.
// The slot is the parameter to reference the different textures in GPU
void Texture::bind(unsigned int slot) const // 
{
    // glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}