#include "Window.h"

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{   
    // LOG4CXX_ERROR(loggerOpenGL, "GLFW Error ("
    // << error << "): " << description);
}

Window::Window(const WindowData& windowData) 
    :m_WindowData(windowData)
{
    if(!s_GLFWInitialized)
    {
        int sucess = glfwInit();
        //FIXME:assert??
        if(!sucess)
        //     LOG4CXX_ERROR(loggerOpenGL, "ERROR: could not start GLFW3")
		// glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }
    

    // Create a glfw window and is stored
    m_GLFWWindow = glfwCreateWindow((int) m_WindowData.Width, (int) m_WindowData.Height, m_WindowData.Title.c_str(), nullptr, nullptr);
    if(!m_GLFWWindow)
    {
        // LOG4CXX_ERROR(loggerOpenGL, "Failed to create a GLFW Window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_GLFWWindow);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
        // LOG4CXX_ERROR(loggerOpenGL, "Error with the glewInit");
        
    // LOG4CXX_INFO(loggerOpenGL, "--- OpenGL Info ---");
    // LOG4CXX_INFO(loggerOpenGL, "Vendor: " << glGetString(GL_VENDOR));
    // LOG4CXX_INFO(loggerOpenGL, "Renderer: " << glGetString(GL_RENDERER));
    // LOG4CXX_INFO(loggerOpenGL, "Version: " << glGetString(GL_VERSION));

    glfwSetWindowUserPointer(m_GLFWWindow, &m_WindowData);
    setVSync(true);

    // TODO: Definition of the callbacks you need for OpenGL
    // glfwSetWindowSizeCallback();
    // glfwSetWindowCloseCallback();
    // glfwSetKeyCallback();
}


void Window::shutdown()
{
    glfwDestroyWindow(m_GLFWWindow);
}

void Window::update()
{
    glfwPollEvents(); // call for the possible events have taken place
    glfwSwapBuffers(m_GLFWWindow);
}

void Window::setVSync(bool enabled)
{
    if(enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    
    m_WindowData.VSync = enabled;
}

bool Window::isVSync() const
{
    return m_WindowData.VSync;
}

Window* Window::create(const WindowData& windowData)
{
    //TODO: delte this new memory or use smart pointers
    return new Window(windowData);
}

