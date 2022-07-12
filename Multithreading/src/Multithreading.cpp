#include "Multithreading.h"



// Multithreading::getBackgroundVertices(float *vertices)
// {
//     vertices = {
//         this->originX        
//     }

// }

Multithreading::Multithreading()
{
    this->originX = 100;
    this->originY = 600;
    this->originZ = 1;
    this->width = 1100;
    this->height = 200;

    // this should have been taken from this->background and this->foreground and convert to the original type. (just /255.0f)
    m_BackgroundColor = {1.0f, 0.0f, 0.0f, 1.0f};
    m_BorderColor= {1.0f, 0.05f, 0.5f, 1.0f};
    this->lineWidht = 20;

    // information
    m_RWY = L"78";
    m_WindDirection = L"360º";
    m_WindSpeed_Gust = L"15/27";


    //TODO: take into account
    // m_FontTitle.init("/home/jjjurado/Dev/OpenGL/Multithreading/res/fonts/FUTRFW.TTF", 25); // the Size of the font is stored in the oglVisualCommon class
    // m_FontData.init("/home/jjjurado/Dev/OpenGL/Multithreading/res/fonts/FUTRFW.TTF", 40); // the Size of the font is stored in the oglVisualCommon class
    
    m_FontTitle.init("/home/jjjurado/Dev/OpenGL/Multithreading/res/fonts/secrcode.ttf", 25); // the Size of the font is stored in the oglVisualCommon class
    m_FontData.init("/home/jjjurado/Dev/OpenGL/Multithreading/res/fonts/secrcode.ttf", 40); // the Size of the font is stored in the oglVisualCommon class

    // TODO: choose if this is the real moment to declare this 
    static RectOgl rectOglAux;
    rectOglAux.left =  0;
    rectOglAux.right = WINDOW_WIDTH;
    rectOglAux.bottom = 0;
    rectOglAux.top = WINDOW_HEIGHT;
    m_recOgl_ptr = &rectOglAux;



}

void Multithreading::attach()
{
    //TODO: enable debugging. It is done in the beginning
    // EnableGlDebugging();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//TODO: change the location of the fragments shader
    m_Shader = std::make_unique<Shader>(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.vs"), 
        Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.fs"));
    
    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    // TODO: avoid hardcoding
    int vertices[] = 
    {   // Background position
        originX,            originY,            originZ,
        originX,            originY - height,   originZ,
        originX + width,    originY - height,   originZ,
        originX + width,    originY,            originZ,
        // Border position
        originX - lineWidht,            originY + lineWidht,            originZ,
        originX - lineWidht,            originY - height - lineWidht,   originZ,
        originX + width + lineWidht,    originY - height - lineWidht,   originZ,
        originX + width + lineWidht,    originY + lineWidht,            originZ,
        // Border in the middle
        originX + width/3,              originY,            originZ,
        originX + width/3,              originY - height,   originZ,
        originX + width/3 + lineWidht,  originY - height,   originZ,
        originX + width/3 + lineWidht,  originY,            originZ,
        // 

    };
                          
    uint32_t indices[] = {0, 1, 2, 2, 3, 0, //background
                         4, 5, 6, 6, 7, 4,  // border position
                         8, 9, 10, 10, 11, 8};
    // uint32_t indicesBackground[] = {4, 5, 6, 6, 7, 4};

    glCreateBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, sizeof(int) * 3, 0);
    glCreateBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void Multithreading::detach()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Multithreading::update(double ts)
{
    m_Shader->use();

    glm::mat4 mvp = glm::mat4(1.0f);
    glm::mat4 proj = glm::ortho( 0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, -1.0f, 1.0f);   // projects the view into a field of (16 x 16 normalized)
    // glm::mat4 model = glm::scale();
    mvp = mvp * proj;
	m_Shader->setUniformMatrix4fv("u_MVP", mvp);

    glBindVertexArray(m_VAO);

    // draw Border internal
    m_Shader->setUniform4f("u_Color", m_BorderColor);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t)*12));
    // draw Brackground
    m_Shader->setUniform4f("u_Color", m_BackgroundColor);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    // draw Border
    m_Shader->setUniform4f("u_Color", m_BorderColor);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t)*6));

    m_Shader->unbind();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glColor3f(1.f, 1.f, 1.f);
    // freetypeV::print (m_FontData, 0, 0, m_recOgl_ptr, L"Hello World!");
    // freetypeV::print (m_FontData, 100, 200, m_recOgl_ptr, L"First Comment");
    // freetypeV::print (m_FontData, 100, 300, m_recOgl_ptr, L"Second Comment");
    // freetypeV::print (m_FontData, 100, 400, m_recOgl_ptr, L"Third Comment");
    // freetypeV::print (m_FontData, 100, 500, m_recOgl_ptr, L"Fourth Comment");
    //TODO: do not use magic numbers
    freetypeV::print (m_FontTitle, originX + 10, originY - 30, m_recOgl_ptr, L"RWYPº");
    freetypeV::print (m_FontTitle, originX + width/3 + lineWidht + 10, originY - 30, m_recOgl_ptr, L" Wind Direction\u00B0", L"HELLO");
    freetypeV::print (m_FontTitle, originX + 2*width/3 + lineWidht, originY - 30, m_recOgl_ptr, L"Wind Speed/Gust");
    
    glPushMatrix();
    // glScalef (2.0f, 2.0f, 1.0f);
    const wchar_t* widecstr = m_RWY.c_str();
    freetypeV::print (m_FontData, originX + 30, originY - 2*height/3, m_recOgl_ptr, widecstr);
    widecstr = m_WindDirection.c_str();
    freetypeV::print (m_FontData, originX + width/3 + lineWidht + 30, originY - 2*height/3, m_recOgl_ptr, widecstr);
    widecstr = m_WindSpeed_Gust.c_str();
    freetypeV::print (m_FontData, originX + 2*width/3 + lineWidht + 30, originY - 2*height/3, m_recOgl_ptr, widecstr);
    glPopMatrix();



    
}

void Multithreading::printHelloWorld()
{
    std::cout << "Hello World!" << std::endl;
}