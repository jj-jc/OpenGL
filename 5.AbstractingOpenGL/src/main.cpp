/**************************************************************
**************************************************************
**
** $RCSfile: Simple.ccp $
**
** $Author: jj-jc $
**
** $Revision: 0.0.0 $
**
** $Date: 2022/02/14 12:21 $
**
** COPYRIGHT: .
** All the rights reserved
**
** Description: This a simple example of using OpenGL. The tutorial
** Adopted is related with 'The Cherno' channel and 'http://www.opengl-tutorial.org/es/'
**
**************************************************************
**************************************************************/

// Include of the GLEW. always first than the glfw3
#include <GL/glew.h>

// Include of the GLFW
#include <GLFW/glfw3.h>

// Standard headers
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <string.h>

// log4cxx 
#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/pool.h"

// imgui lib
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// My files
#include "Shader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"

// OpenGL math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Convertion of images into buffer information
#include "stb_image.h"

// Tests
#include "tests/TestClearColor.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// The creation of Loggers must be outside of int main funciton(){}
log4cxx::LoggerPtr loggerMain = log4cxx::LoggerPtr (log4cxx::Logger::getLogger ("Stitching")); // definition of static variable

int main(void)
{
// Logger references and configurations
log4cxx::File pc ("/home/jjjurado/Dev/OpenGL/5.AbstractingOpenGL/conf/log4.cxx.properties");    
log4cxx::BasicConfigurator::resetConfiguration ();
log4cxx::PropertyConfigurator::configure (pc);

// LOG4CXX_INFO(loggerMain, "-------- LOGGER MAIN --------" << "\n");
// LOG4CXX_WARN(loggerMain, "-------- LOGGER MAIN --------" << "\n");
// LOG4CXX_DEBUG(loggerMain, "-------- LOGGER MAIN --------" << "\n");
// LOG4CXX_ERROR(loggerMain, "-------- LOGGER MAIN --------" << "\n");
// LOG4CXX_TRACE(loggerMain, "-------- LOGGER MAIN --------" << "\n");

#ifdef MY_DEBUG
std::cout << "------------------ Debug Mode ------------------" << std::endl;
#endif


    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
      fprintf(stderr, "ERROR: could not start GLFW3\n");
      return 1;
    } 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
#ifdef MY_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    /* Create a windowed mode window and its OpenGL context */
    // ------------------------------------------------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "5.AbstractingOpenGL", NULL, NULL);
    if (!window)
    {
    	std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    /* This has to be done after make a valid OpenGL rendering context */
    // start GLEW extension handler
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark(); // or  ImGui::StyleColorsClassic();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // setting GL_DEBUG_OUTPUT
#ifdef MY_DEBUG
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        std::cout << "GL_DEBUG_OUTPUT setted correctly" << std::endl;
    }
#endif

    // --------------------------------------
    // OpenGl tools
    //
    // For using blend feature
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glfwSetKeyCallback(window, key_callback);

    float positions[] = {
        // positions         // colors          // texture coordanates
         300.0f, 100.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,     // bottom right
         100.0f, 100.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,     // bottom left
         100.0f, 300.0f, 0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,     // top left
         300.0f, 300.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f      // top right
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Vertex buffer object
    VertexBuffer vbo(positions, sizeof(positions));
    // Vertex array object
    VertexArray vao;
    VertexBufferLayout layout;
    layout.push<float>(3); // positions
    layout.push<float>(3); // color
    layout.push<float>(2); // texture coordenates
    vao.addBuffer(vbo, layout);
    // Index buffer object
    IndexBuffer ibo(indices, sizeof(indices));

    // MVP matrix
    glm::vec3 translationA(0.0f, 0.0f, 0.0f);
    glm::vec3 translationB(0.0f, 0.0f, 0.0f);
    glm::vec3 scale(1.0f, 1.0f, 1.0f);
    glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f); // projects the view into a field of (16 x 16 normalized)
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0,0)); // translate the object to the left or translate the camera to the right
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = proj * view * model;

    // Create a complete shader program (with vertex and fragment shaders)    
    Shader myShader(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/5.AbstractingOpenGL/res/shaders/triangle.vs"), 
                    Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/5.AbstractingOpenGL/res/shaders/triangle.fs"));
    myShader.bind();
    // myShader.setUniform4f("u_Color", glm::vec4(0.2627, 0.8706, 0.1098, 1.0));
    // load any kind of image to the GPU
    Texture texture("/home/jjjurado/Dev/OpenGL/5.AbstractingOpenGL/res/textures/Linux.jpeg"); 
    texture.bind(0); // bind to the slot we want to use.
    myShader.setUniform1i("u_Texture", 0); // this set to the slot of the texture we want to use in the uniform
    vao.unbind();
    vbo.unbind();
    ibo.unbind();
    myShader.unbind();  
    Renderer myRenderer;

    // Our state
    ImVec4 clear_color = ImVec4(0.0471, 0.5137, 0.6549, 1.0);
    int display_w, display_h; // variables for glfwGetFramebufferSize() and control de size of the windows;

    test::TestClearColor test;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        myRenderer.clear();

        test.OnUpdate(0.0f);
        test.OnRender();

        myShader.bind();
        // Use the polygon mode. This affects how the objects are rasterized (4th step in the magic plumb)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // works to debug if everything is drawing as it is suppose to
        // myShader.setUniform4f("u_Color", glm::vec4(0.8706, 0.7961, 0.1098, 0.5));
        
        {
            model = glm::translate(glm::mat4(1.0f), translationA);
            // model = glm::translate(glm::mat4(1.0f), translation) * glm::scale(translation, scale);
            mvp = proj * view * model;
            myShader.setUniformMatrix4fv("u_MVP", mvp);
            myRenderer.draw(vao, ibo, myShader); // Although the vao stores the ibo too, it is more flexible if it is possible to change that information
        }

        {
            model = glm::translate(glm::mat4(1.0f), translationB);
            // model = glm::translate(glm::mat4(1.0f), translation) * glm::scale(translation, scale);
            mvp = proj * view * model;
            myShader.setUniformMatrix4fv("u_MVP", mvp);
            myRenderer.draw(vao, ibo, myShader); // Although the vao stores the ibo too, it is more flexible if it is possible to change that information
        }

        // select and draw the element buffer
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw with the element information, if it has no information. Segment fault (core dumped)
        
        // --------------------------------------
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        test.OnImGuiRender();

        // 1. Show the big demo window
        ImGui::ShowDemoWindow((bool*)true);
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            // ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::SliderFloat2("translationA", &translationA.x, 0, SCR_WIDTH);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat2("translationB", &translationB.x, 0, SCR_WIDTH);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat2("Scale", &scale.x, 0, 1);            // Edit 1 float using a slider from 0.0f to 1.0f //FIXME:magic number
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // // OpenGL process
        // glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
        //             clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        // Swap front and back buffers
        glfwSwapBuffers(window); 
        // Poll for and process events
        glfwPollEvents(); 
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate(); // Close OpenGlL window and terminate GLFW context 
    exit(EXIT_SUCCESS);
}


// Definitions of functions 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    	std::cout << "Esc key pressed" << std::endl;
    }
}