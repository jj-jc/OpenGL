#ifndef _TEST_FREE_TYPE_H_
#define _TEST_FREE_TYPE_H

#include <iostream>
#include <memory>
#include <map>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "CSU_OPENGL/Shader.h"
#include "CSU_OPENGL/VertexBuffer.h"
#include "CSU_OPENGL/VertexArray.h"
#include "CSU_OPENGL/IndexBuffer.h"
#include "CSU_OPENGL/Texture.h"
#include "CSU_OPENGL/Shader.h"
#include "Test.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace test{
    
struct Character{ // Stores the font characters informations
    unsigned int textureID; // ID handle of the glyph texture
    glm::ivec2 size;        // Size of glyph
    glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
    unsigned int advance;   // Offset to advance to next glyph
};

class TestFreeType : public Test
{
    public:
        TestFreeType();
        ~TestFreeType();

        void OnUpdate(float deltaTime) override;
        void OnRender();
        void OnImGuiRender() override;
        int loadFont(std::string fontPath);

    private:
        std::string m_Text;
        glm::vec4 m_Pos;
        glm::vec3 m_Color;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<Texture> m_Texture;
        std::unique_ptr<Shader> m_Shader;
        std::map<GLchar, Character> Characters;

};



}

#endif