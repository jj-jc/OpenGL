#ifndef _IMGUI_LAYER_H_
#define _IMGUI_LAYER_H_

#include "CSU_MANAGER/Layer.h"
#include "CSU_MANAGER/Window.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

// #include "CSU_OPENGL/VisualManager.h"

#include <GLFW/glfw3.h>

void HelpMarker(const char* desc);

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ImGuiLayer(Window* window);
    ~ImGuiLayer() = default;

    virtual void attach() override;
    virtual void detach() override;

    void begin();
    void end();

    // virtual void event(Ev)
    // bool isMouseButtonPressed();
    inline Window* getGLFWwindow() const {return m_Window;}
private:
    float m_Time = 0.0f;
    Window* m_Window;
};


#endif