#ifndef WIND_PANEL_H_
#define WIND_PANEL_H_

#include "CSU_OPENGL/Shader.h"
#include "CommLoop.h"
#include "CSU_MANAGER/Layer.h"
#include "CSU_OPENGL/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FreeTypeV.h"


#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280


struct RGBA {
    unsigned char r, g, b, a;
};



class Multithreading : public torre::CommLoop::PanoramicEntityH, public Layer
{
    // Limits 
    const static uint32_t MaxWindSpeed;
    const static uint32_t HighTailWind;
    const static uint32_t ExtremTailWind;
    // Colors 
    const static RGBA HighTailWindColor;
    const static RGBA ExtremTailWindColor;
    const static RGBA OpenBackgroundColor;  // Background's color when the runway is open
    const static RGBA ClosedBackgroundColor;// Background's color when the runway is closed
    // OpenGL object identifiers
    GLuint m_VAO, m_VBO, m_EBO; // the three ids are necesary to store to be able to delete in the detach method
    std::unique_ptr<Shader> m_Shader; 
    glm::vec4 m_BackgroundColor;
    glm::vec4 m_BorderColor;
    // GLuint m_VAOBackground, m_VBOBackground, m_EBOBackground;
    std::wstring m_RWY, m_WindDirection, m_WindSpeed_Gust;


// TODO: take into account
    // variables not needed but to make a test
    freetypeV::font_data m_FontTitle;
    freetypeV::font_data m_FontData;
    RectOgl* m_recOgl_ptr;



public:
    Multithreading();
    // getBackgroundVertices(float *vertices);
//     getForegroundVertices(float *vertices);

    virtual void attach() override;
    virtual void detach() override;
    virtual void update(double ts) override;
    virtual void printHelloWorld() override;
    // void renderImGui();

};

#endif /* WIND_PANEL_H_*/