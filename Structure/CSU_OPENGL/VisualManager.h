#ifndef _VISUAL_MANAGER_H_
#define _VISUAL_MANAGER_H_

#include <string>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "CSU_IMGUI/ImGuiLayer.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Window.h"

class VisualManager
{
public:
    VisualManager(const std::string& name = "OpenGL Application", uint32_t width = 1280, uint32_t height = 720);
    virtual ~VisualManager() = default;

    void run();
    // void OnEvent(Event& e);
    void pushLayer(Layer* layer);
    void pushOverLayer(Layer* layer);

    inline Window& getWindow() {return *m_Window;}
    inline Window* getWindowPtr() {return m_Window;}
    inline static VisualManager& getVisualManager() {return *s_Instance;}

private:
    // bool closeWindow(Event& e);
private:
    // make it unique
    Window* m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;  //this store if VisualManager is running or not.
    LayerStack m_LayerStack;
    float m_LastFrameTime = 0.0f;
private:
    static VisualManager* s_Instance;
};

#endif