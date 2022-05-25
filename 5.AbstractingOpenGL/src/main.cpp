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
** COPYRIGHT: INDRA SISTEMAS, 2000.
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

// imgui lib
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// My files
#include "Shader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;




int main(void)
{
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
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
    // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.0471, 0.5137, 0.6549, 1.0);

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

    /* Definitions of objects */
    float positions[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,    0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,    0.0f,    // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,    1.0f,    // top left
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,    0.0f     // top right
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
    layout.push<float>(3);
    layout.push<float>(3);
    vao.addBuffer(vbo, layout);

    // Element buffer object
    IndexBuffer ibo(indices, sizeof(indices));

    // // Vertex Attribute position
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // // Vertex Attribute colors
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    // // Vertex array object 2
    // unsigned int vao2;
    // glGenVertexArrays(1, &vao2);
    // glBindVertexArray(vao2);
    // // vbo.bind();
    // // glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // // Vertex Attribute position
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // // Vertex Attribute colors
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    // // glBindVertexArray(0);


    // glBindVertexArray(vao);

    // Create a complete shader program (with vertex and fragment shaders)    
    Shader ourShader("/home/jjjurado/Dev/OpenGL/5.AbstractingOpenGL/res/shaders/triangle.vs", "/home/jjjurado/Dev/OpenGL/5.AbstractingOpenGL/res/shaders/triangle.fs");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        // Use the polygon mode. This affects how the objects are rasterized (4th step in the magic plumb)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // // select and draw the vertex array
        vao.bind();
        // glDrawArrays(GL_TRIANGLES, 0, 3);   

        // select and draw the element buffer
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw with the element information, if it has no information. Segment fault (core dumped)

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in
        // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
        // ImGui!).
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                    clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



        // draw our first triangle
        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
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
