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
#include <cmath>
#define PI 3.1415

// Include own header files
#include "Shader.hpp"

// Includes for ImGui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 720;

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void error_callback(int error, const char* description);
static ShaderProgramSource ParseShader(const std::string& filepath);
static unsigned int CompileShader(unsigned int type, const std::string& source);
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void coutPos(ImVec2 pos, const char *variable);
static void DebugLogInputEvent(const ImGuiInputEvent* e);
void ImGui_renderUi();
void createMenu();


int main(void)
{
	std::cout << "ImGui example:" << std::endl;

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	/* Initialize the library */
    if (!glfwInit())
    	exit(EXIT_FAILURE);

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Create a windowed mode window and its OpenGL context */
    // ------------------------------------------------------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello ImGui", NULL, NULL);
    if (!window)
    {
    	std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window); // Make the window's context
    glfwSwapInterval(1); // activate v-sync. To synchronize the frames of the screen
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    ImGuiContext *myContext;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    myContext = ImGui::CreateContext();
    ImGuiIO&  io = ImGui::GetIO(); (void)io;

    io.IniFilename = "resources/imgui.ini";
    io.MouseDrawCursor = true;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true); // install callbacks to some functions that will catch the keyboard events
    ImGui_ImplOpenGL3_Init(glsl_version);

    /* This has to be done after make a valid OpenGL rendering context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Set up and creation the shader program */
    // ------------------------------------------------------------------
    ShaderProgramSource source = ParseShader("resources/shaders.glsl");
    /*
    std::cout << "VERTEX:" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT:" << std::endl;
    std::cout << source.FragmentSource << std::endl;
    */
    unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);


    ////////////////////////////////
    /* Control of the flow events */
    ////////////////////////////////
    // 			Inputs
    // Keys
    glfwSetKeyCallback(window, key_callback);

    // Mouse
//    glfwSetMouseButtonCallback(window, mouse_button_callback);


    /* set up vertex data (and buffer(s)) and configure vertex attributes */
    // ------------------------------------------------------------------
    float vertices[] = {
    	 // Position 			// color
         0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.0f, 	0.0f, 0.0f, 1.0f,// bottom left
        -0.5f,  0.5f, 0.0f, 	0.0f, 0.0f, 0.0f // top left
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // this kind of object stores the info of the buffer and configuration of the attributes
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    //Shader shader("Shaders/shader.vs", "Shaders/shaders.fs");

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    /* Loop until the user closes the window */



    static float rotation = 0.0f;
    static float translation[] = { 0.0f, 0.0f};
    static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f};
    ImVec2 localMousePos = {0.0f ,0.0f};
    ImVec2 windowMousePos = {0.0f ,0.0f};
    ImGuiMouseCursor mouseCursor;
    int counter = 0;

    // Our state
    bool show_demo_window = true;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//    ImGuiContext& g = *GImGui;

    while (!glfwWindowShouldClose(window))
    {
    	// pre-render
        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // render
//		ImGui::UpdateInputEvents(false);

        // be sure to activate the shader
        glUseProgram(program);

    	float timeValue = glfwGetTime();
    	float greenValue = sin(timeValue) / 2.0f + 0.5f;
    	int vertexColorLocation = glGetUniformLocation(program, "ourColor");
    	int rotationID = glGetUniformLocation(program, "rotation");
    	int translationID = glGetUniformLocation(program, "translation");

        // draw our first triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no need to un-bind it every time

        // render your GUI
//        ImGui::Begin("Demo window", );
//        ImGui::Begin("Demo", 0);
//        ImGui::Button("Hello!");
//        ImGui::SliderFloat("rotation", &rotation, 0, 2 * PI);
//        //ImGui::Spacing();
//        //ImGui::NewLine();
//        ImGui::SliderFloat2("position", translation, -1.0, 1.0);
//        ImGui::End();

        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glUniform1f(rotationID, rotation);
        glUniform2f(translationID, translation[0], translation[1]);

        // Probar cosas con Dear imGui
//        ImGui::ShowUserGuide();
//        mouseCursor = ImGui::GetMouseCursor();
//
//        localMousePos = ImGui::GetMousePos();


//        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//        if (show_demo_window)
//            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

		if(ImGui::IsKeyPressed(ImGuiKey_1, false))
		{
			std::cout << "R pressed" << std::endl;
			io.MousePos = {(float)40.0f * timeValue, (float)(300.0f + 200.0f*sin(timeValue))};
		}

		if(ImGui::IsKeyPressed(ImGuiKey_Space, false))
		{
			std::cout << "Key Event simulated" << std::endl;
		}

		if(ImGui::IsMouseClicked(0, false))
		{
			std::cout << "Mouse Click Event simulated" << std::endl;
		}


		ImGui_renderUi();
//		createMenu();

        // Render dear imgui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Aquí es donde se renderiza realmente por medio de OpenGL

        glfwSwapBuffers(window); // Swap front and back buffers
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);
    glfwDestroyWindow(window);


    glfwTerminate(); // Close OpenGlL window and terminate GLFW
    exit(EXIT_SUCCESS);
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static ShaderProgramSource ParseShader(const std::string& filepath) {
    /* Open the file */
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                // set mode vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                // set mode fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    /* Create the shader */
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    /* Error handling */
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // This code says the compiler to store memory dinamically in stack
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader!" << std::endl;
        std::cout << message << std::endl;

        /* The compilation didnt work out */
        glDeleteShader(id);

        return 0;
    }

    return id;
}
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /* This is like a program in c */
    /* Firs create the file */
    unsigned int program = glCreateProgram();

    /* Compile the files */
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* Link the files into the same program */
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    /* Delete the memory of every file */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
    	std::cout << "Enter pressed" << std::endl;
    	ImGuiIO& io = ImGui::GetIO();
    	io.AddKeyEvent(ImGuiKey_Space, true);
        io.AddKeyEvent(ImGuiKey_Space, false);
        io.AddMouseButtonEvent(0, true);
        io.AddMouseButtonEvent(0, false);
    }

    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
    	std::cout << "Left Key pressed" << std::endl;
    	ImGuiIO& io = ImGui::GetIO();
    	ImVec2 oldMousePos = ImGui::GetMousePos();
    	io.AddMousePosEvent(oldMousePos.x - 20.0f, oldMousePos.y);
    }

    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
    	std::cout << "Right Key pressed" << std::endl;
    	ImGuiIO& io = ImGui::GetIO();
    	ImVec2 oldMousePos = ImGui::GetMousePos();
    	io.AddMousePosEvent(oldMousePos.x + 20.0f, oldMousePos.y);
    }

    if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
    	std::cout << "Right Key pressed" << std::endl;
    	ImGuiIO& io = ImGui::GetIO();
    	ImVec2 oldMousePos = ImGui::GetMousePos();
    	io.AddMousePosEvent(oldMousePos.x, oldMousePos.y - 20.0f);
    }

    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
    	std::cout << "Right Key pressed" << std::endl;
    	ImGuiIO& io = ImGui::GetIO();
    	ImVec2 oldMousePos = ImGui::GetMousePos();
    	io.AddMousePosEvent(oldMousePos.x, oldMousePos.y + 20.0f);
    }

    if(key == GLFW_KEY_A && action == GLFW_PRESS)
    {
    	std::cout << "Right Key pressed" << std::endl;
    	ImGuiIO& io = ImGui::GetIO();
    	float timeValue = glfwGetTime();
    	io.AddMousePosEvent((float)40.0f * timeValue, (float)(300.0f + 200.0f*sin(timeValue)));
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    	std::cout << "Right Mouse Input pressed" << std::endl;
}

void coutPos(ImVec2 pos, const char *variable)
{
	std::cout << "---" << variable << "position --- "<< std::endl;
	std::cout << "x: " << pos.x << "y: " << pos.y << std::endl;
}

static void DebugLogInputEvent(const ImGuiInputEvent* e)
{
    switch (e->Type)
    {
    case ImGuiInputEventType_MousePos:
        IMGUI_DEBUG_LOG_IO("Event: MousePos (%.1f %.1f)\n", e->MousePos.PosX, e->MousePos.PosY);
        break;
    case ImGuiInputEventType_MouseButton:
        IMGUI_DEBUG_LOG_IO("Event: MouseButton %d %s\n", e->MouseButton.Button, e->MouseButton.Down ? "Down" : "Up");
        break;
    case ImGuiInputEventType_MouseWheel:
        IMGUI_DEBUG_LOG_IO("Event: MouseWheel (%.1f %.1f)\n", e->MouseWheel.WheelX, e->MouseWheel.WheelX);
        break;
    case ImGuiInputEventType_Key:
        IMGUI_DEBUG_LOG_IO("Event: Key \"%s\" %s\n", ImGui::GetKeyName(e->Key.Key), e->Key.Down ? "Down" : "Up");
        break;
    case ImGuiInputEventType_Focus:
        IMGUI_DEBUG_LOG_IO("Event: AppFocused %d\n", e->AppFocused.Focused);
        break;
    case ImGuiInputEventType_None:
    default:
        break;
    }
}

void createMenu(){
	bool my_tool_active;
	float my_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	// Create a window called "My First Tool", with a menu bar.
	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
	    if (ImGui::BeginMenu("File"))
	    {
	        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
	        if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
	        if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
	        ImGui::EndMenu();
	    }
	    ImGui::EndMenuBar();
	}

	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", my_color);

	// Plot some values
	const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
	    ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
}


void ImGui_renderUi(){
	ImGuiIO& io {ImGui::GetIO()};
	static bool dbgWindow {true};
	static bool demoWindow {true};
	static float value = 0.0f;


	{ // main window
		static bool showfps {true};
		ImGui::Begin("Window test");
		if (ImGui::Button("Button")){
			if (showfps) showfps = false;
			else showfps = true;
		}
		if(showfps)
			ImGui::Text("FPS: %.2f", io.Framerate);
		else
			ImGui::Text("ms x frame: %.2f", 1000.0f / io.Framerate);

		ImGui::Checkbox("Debug Window", &dbgWindow);
		ImGui::Checkbox("Demo Window", &demoWindow);

		ImGui::End();
	}

	if (dbgWindow){ // Debug window
		ImGui::Begin("Debug Window");
		ImGui::SliderFloat("FPS", &value, 1, 100, "%.2f");
		ImGui::End();
	}

	if (demoWindow){
		ImGui::ShowDemoWindow();
	}

}

