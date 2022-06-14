#ifndef _EXAMPLE_LAYER_H_
#define _EXAMPLE_LAYER_H_

// Include of the GLEW. always first than the glfw3
#include <GL/glew.h>

// Include of the GLFW
#include <GLFW/glfw3.h>

// Standard headers
#include <iostream>
// #include <fstream>
// #include <string>
// #include <sstream>
// #include <stdio.h>
// #include <string.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// imgui lib
#include "imgui/imgui.h"
// #include "imgui/backends/imgui_impl_glfw.h"
// #include "imgui/backends/imgui_impl_opengl3.h"

#include "CSU_OPENGL/Layer.h"
#include "Shader.h"


class ExampleLayer : public Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer();

    virtual void attach() override;
    virtual void detach() override;
    virtual void update(double ts) override;
    virtual void renderImGui() override;
private:
    Shader* m_Shader;
	GLuint m_QuadVA, m_QuadVB, m_QuadIB;

	glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_SquareAlternateColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor = m_SquareBaseColor;
};

#endif