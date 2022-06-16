#include "VisualManager.h"
// TODO: delete this line and try to include in the .h file
// #include "Log.h"

// #include "CSU_IMGUI/ImGuiLayer.h"

VisualManager* VisualManager::s_Instance = nullptr;

VisualManager::VisualManager(const std::string& name, uint32_t width, uint32_t height)
{
    if(!s_Instance)
    {
        // Initializer core
        // LOG_INIT("/home/jjjurado/Dev/OpenGL/VisualObjects/conf/log4.cxx.properties");
    }

    s_Instance = this;
    WindowData windowData(name, width, height);
    this->m_Window = (Window::create((const WindowData) windowData));
    // m_Window->setEventCallback

    m_ImGuiLayer = new ImGuiLayer(this->getWindowPtr());
    pushLayer(m_ImGuiLayer);
}

void VisualManager::pushLayer(Layer* layer)
{
    m_LayerStack.pushLayer(layer);
}

void VisualManager::pushOverLayer(Layer* layer)
{
    m_LayerStack.pushOverLayer(layer);
}

void VisualManager::popLayer(Layer* layer)
{
    m_LayerStack.popLayer(layer);
}

void VisualManager::popOverLayer(Layer* overlayer)
{
    m_LayerStack.popOverLayer(overlayer);
}

void VisualManager::run()
{
    while(m_Running)
    {
        float time = (float)glfwGetTime();
        float timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        for(Layer* layer : m_LayerStack)
            layer->update(timestep);
        
        m_ImGuiLayer->begin();
        for (Layer* layer : m_LayerStack)
            layer->renderImGui();
        m_ImGuiLayer->end();

        m_Window->update();
    }
}