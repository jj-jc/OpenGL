#include "TestFreeType.h"


namespace test{


    TestFreeType::TestFreeType()
        :m_Text{"Hello World!"}, m_Pos{100.5f, 100.5f, 0.0f, 1.0f}, m_Color{0.5, 0.8f, 0.2f}
    {
        if(!loadFont("/home/jjjurado/Dev/OpenGL/VisualObjects/res/fonts/secrcode.ttf"))
            std::cout << "ERROR::TESTFREETYPE: Error loading the font into the Characters structures." << std::endl;
        m_VBO = std::make_unique<VertexBuffer>(nullptr, sizeof(float) * 6 * 4, GL_DYNAMIC_DRAW);
        m_VAO = std::make_unique<VertexArray>();
        VertexBufferLayout layout; // just store the information needed
        layout.push<float>(4);
        m_VAO->addBuffer(*m_VBO, layout); // creates the buffer with the info of layout and the values of vbo

        //FIXME: hardcoded path
        // Create a complete shader program (with vertex and fragment shaders)    
        m_Shader = std::make_unique<Shader>(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/VisualObjects/res/shaders/FreeType.vs"), 
                        Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/VisualObjects/res/shaders/FreeType.fs"));
        m_VAO->unbind();
        m_VBO->unbind();
        m_Shader->unbind();
        
    }

    TestFreeType::~TestFreeType()
    {

    }

    void TestFreeType::OnUpdate(float deltaTime)
    {
        
    }

    void TestFreeType::OnRender()
    {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f); // projects the view into a field of (16 x 16 normalized)
        // activate corresponding render state	
        m_Shader->bind();
        m_Shader->setUniformMatrix4fv("u_MVP", proj);
        m_Shader->setUniform3f("u_TextColor", m_Color);
        glActiveTexture(GL_TEXTURE0);
        m_VAO->bind();
        int x = m_Pos.x;
        int y = m_Pos.y;
        int scale = m_Pos.w;
        // iterate through all characters
        std::string::const_iterator c;
        for (c = m_Text.begin(); c != m_Text.end(); c++)
        {

            Character ch = Characters[*c];

            float xpos = x + ch.bearing.x *scale;
            float ypos = y - (ch.size.y - ch.bearing.y) * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            // update content of VBO memory
            m_VBO->bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            m_VBO->unbind();
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64) // be carefull with this!!!
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        m_Shader->unbind();
        m_VAO->unbind();
    }

    void TestFreeType::OnImGuiRender()
    {
        
    }



    int TestFreeType::loadFont(std::string fontPath)
    {
        // Create a library
        FT_Library ft;
        if(FT_Init_FreeType(&ft))
        {
            //FIXME couts
            std::cout << "Error::FREETYPE: Could not init FreeType Library" << std::endl;
            return -1;
        }
        // Load a font into the library. A face hosts a collection of glyphs
        FT_Face face;//FIXME: hardcoded path
        if(FT_New_Face(ft, fontPath.c_str(), 0, &face))
        {
            //FIXME:couts
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return -1;
        }
        // Define the pixel font size to extract from this face:
        FT_Set_Pixel_Sizes(face, 0, 48); // 0 width and 48 hight ( width will be dynamically calculated)
        // Set one of those glyphs as active. This creates a 8-bit grayscale bitmap image for us. (face->glyph->bitmap)
        if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
        {//FIXME: couts
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            return -1;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return 1;
    }

}
