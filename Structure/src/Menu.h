#ifndef _MENU_LAYER_H_
#define _MENU_LAYER_H_

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
#include "MetPanelLayer.h"


extern log4cxx::LoggerPtr loggerMenu;

class Menu : public Layer
{
public:
    Menu();
    Menu(const std::string& name);
    virtual ~Menu();

    virtual void attach() override;
    virtual void detach() override;
    virtual void update(double ts) override;
    virtual void renderImGui() override;
};

#endif