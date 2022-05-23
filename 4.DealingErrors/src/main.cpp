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
#include "shader.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main(void)
{

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
      fprintf(stderr, "ERROR: could not start GLFW3\n");
      return 1;
    } 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Create a windowed mode window and its OpenGL context */
    // ------------------------------------------------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "4.DealingErrors", NULL, NULL);
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

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can
    // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
    // them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
    // need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please
    // handle those errors in your application (e.g. use an assertion, or display
    // an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored
    // into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
    // ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string
    // literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font =
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
    // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.0471, 0.5137, 0.6549, 1.0);

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
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    // Vertex array object
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // Vertex Attribute position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // Vertex Attribute colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    // Vertex array object 2
    unsigned int vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // Vertex Attribute position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // Vertex Attribute colors
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    // glBindVertexArray(0);

    // Element buffer object
    glBindVertexArray(vao);
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    


    Shader ourShader("/home/jjjurado/Dev/OpenGL/4.DealingErrors/res/shaders/triangle.vs", "/home/jjjurado/Dev/OpenGL/4.DealingErrors/res/shaders/triangle.fs");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        // Use the polygon mode. This affects how the objects are rasterized (4th step in the magic plumb)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // // select and draw the vertex array
        glBindVertexArray(vao);
        // glDrawArrays(GL_TRIANGLES, 0, 3);   

        // select and draw the element buffer
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_INT, 0);

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
    glfwTerminate(); // Close OpenGlL window and terminate GLFW
    exit(EXIT_SUCCESS);
}
