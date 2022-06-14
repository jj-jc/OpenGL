#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "log4cxx/log4cxx.h"
#include "log4cxx/logger.h"

// extern log4cxx::LoggerPtr loggerOpenGL;

struct WindowData
{
public:
    WindowData(const std::string& title = "OpenGL Window", uint32_t width = 1280, uint32_t height = 720)
        :Title(title), Width(width), Height(height){}

    WindowData(const WindowData& windowData)
    {
        Title = windowData.Title;
        Width = windowData.Width;
        Height = windowData.Height;
    }

    std::string Title;
    uint32_t Width, Height;
    bool VSync;
};

class Window
{
public:
    Window(const WindowData& windowData);
    virtual ~Window() = default;

    void update();

    inline uint32_t getWidth() const {return m_WindowData.Width;}
    inline uint32_t getHeight() const {return m_WindowData.Width;}

    // set window attributes (eventcallback), setVsync, isVsync
    // inline void setEventCallback();
    void setVSync(bool enabled);
    bool isVSync() const;

    inline GLFWwindow* getGLFWwindow() const {return m_GLFWWindow;}
    static Window* create(const WindowData& windowData = WindowData());

private:
    // virtual void init(const WindowData& windowData);
    void shutdown(); 
private:
    GLFWwindow* m_GLFWWindow;
    WindowData m_WindowData;
};

#endif