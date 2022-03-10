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
#include <cmath>
#include <stb_image.h> // This manage the conversion of textures in bynaries.
#include <vector>

// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include own header files
#include "Shader.hpp"

// Includes for ImGui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

// Define constants
#define PI 3.1415

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void DoStyle(){
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImColor(100, 100, 100, 255);
	style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
}

static bool Check {false};
int main(void)
{
	std::cout << "Coordinate System example:" << std::endl;
	// ---------------------------------------------------------------------
	// Setting up the context in GLFW and glew	/* Initialize the library */
    if (!glfwInit())
    	exit(EXIT_FAILURE);
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Coordinate System example", NULL, NULL);
    if (!window)
    {
    	std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window); // Make the window's context
    glfwSwapInterval(1); // activate v-sync. To synchronize the frames of the screen

    /* This has to be done after make a valid OpenGL rendering context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    ImGuiContext *myContext;
    // Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DoStyle();
        {
        	static int counter = 0;
        	ImGui::Begin("Testing menu", nullptr, ImGuiWindowFlags_NoCollapse);
        	ImGui::Checkbox("Text box", &Check);
        	if (Check){
        		ImGui::Text("Im Checked");
        		ImGui::SetWindowPos("Testing menu", ImVec2(0,0));
        	}
        	if (ImGui::Button("Click me", ImVec2(30, 30))){
        		counter ++;
        	}
        	ImGui::Text("Counter: %i", counter);
        	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 15));
        	ImGui::Checkbox("Text box", &Check);
        	ImGui::Button("Click me");
        	ImGui::PopStyleVar();

        	ImGui::Columns(2);
        	ImGui::End();
        }

        {
        	ImGui::Begin("Testing Columns", nullptr, ImGuiWindowFlags_NoCollapse);
//        	ImGui::GetWindowID();
        	ImGui::Columns(2);
        	ImGui::SetColumnOffset(1,  ImGui::GetWindowWidth()/2);
        	ImGui::Text("This is the first Column");

        	ImGui::NextColumn();
        	ImGui::Text("This is the second Column");
        	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 20);
        	ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2 + 20);
        	ImGui::Text("This is the second Column2");
        	ImGui::End();
        }

        {
        	ImGui::Begin("Testing Child", nullptr, ImGuiWindowFlags_NoCollapse);
        	ImGui::BeginChild("##LeftSide", ImVec2(ImGui::GetWindowWidth()/2,
        						ImGui::GetWindowHeight() - 50), true);
        	ImGui::EndChild();

        	ImGui::SameLine();
        	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        	ImGui::SameLine();

        	ImGui::BeginChild("##RightSide", ImVec2(ImGui::GetWindowWidth()/2,
        						ImGui::GetWindowHeight() - 50), true);
        	ImGui::EndChild();

        	ImGui::End();
        }

        {
        	static int selectedItem = 0;
        	std::vector<const char*> items = {"Item 1", "Item 2"};

        	ImGui::Begin("Testing the Items");

        	ImGui::PushItemWidth(ImGui::GetWindowWidth() - 40);
        	ImGui::Combo("##ItemsCombo", &selectedItem, items.data(), items.size());
        	ImGui::PopItemWidth();

        	ImGui::End();

        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate(); // Close OpenGlL window and terminate GLFW
    exit(EXIT_SUCCESS);
}

