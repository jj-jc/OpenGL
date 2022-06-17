#ifndef _MET_PANEL_LAYER_H_
#define _MET_PANEL_LAYER_H_

// Include of the GLEW. always first than the glfw3
#include <GL/glew.h>

// Include of the GLFW
#include <GLFW/glfw3.h>

// Standard headers
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// imgui lib
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "CSU_MANAGER/Layer.h"
#include "CSU_MANAGER/VisualManager.h"
#include "CSU_OPENGL/Shader.h"
#include "ExampleLayer.h"


extern log4cxx::LoggerPtr loggerMetPanel;

typedef glm::vec3 pos3d_t;
// typedef pos3d_t vertices_t[4];
typedef float width_t;
typedef float height_t;

class Square
{
private:
    pos3d_t m_pos;
    width_t m_width;
    height_t m_height;
public:
    Square(pos3d_t pos, width_t width, height_t height);
    ~Square() = default;

    inline const pos3d_t* getPos3D() const {return &m_pos;}    
    inline const width_t getWidth() const {return m_width;}    
    inline const height_t getHeight() const {return m_height;}   
    void get4Vertices(pos3d_t* vertices) const;

    void setPos3D(pos3d_t pos3d) {m_pos[0] = pos3d[0]; m_pos[1] = pos3d[1], m_pos[2] = pos3d[2];} 
    void setWidth(width_t width) {m_width = width;} 
    void setHeight(height_t height) {m_height = height;} 
};

class ColorSquare
{


};

class MetPanelLayer : public Layer
{
public:
    MetPanelLayer();
    MetPanelLayer(const std::string& name);
    virtual ~MetPanelLayer();

    virtual void attach() override;
    virtual void detach() override;
    virtual void update(double ts) override;
    virtual void renderImGui() override;
private:
    Shader* m_Shader[3];
	GLuint m_QuadVA[3], m_QuadVB[3], m_QuadIB[3];

	// glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	// glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_EdgeColor = { 1.0f, 0.5f, 0.3f, 1.0f };

    glm::vec3 m_Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Scale = {1.0f, 1.0f, 1.0f};

    // changes
    Square m_BackgroundSquare;
    Square m_ExternalEdgeSquare;
    Square m_InternalEdgeSquare; 
};

#endif