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

// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Include own header files
#include "Shader.hpp"

// Define constants
#define PI 3.1415

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Variables used
const float radius = 10.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
const float cameraSpeed = 0.05f;

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 40.f;
glm::vec3 direction;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// mouse camera control variables
float lastX = 400, lastY = 300;
bool firstMouse = true;


struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

static float visibleLevel = 0.1f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
static ShaderProgramSource ParseShader(const std::string& filepath);
static unsigned int CompileShader(unsigned int type, const std::string& source);
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main(void)
{
	std::cout << "Coordinate System example:" << std::endl;
	// ---------------------------------------------------------------------
	// Setting up the context in GLFW and glew	/* Initialize the library */
    if (!glfwInit())
    	exit(EXIT_FAILURE);
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* This has to be done after make a valid OpenGL rendering context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    //------------------------------------------------------------------------

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // Set up and creation the shader program */
    // ------------------------------------------------------------------
    ShaderProgramSource source = ParseShader("Shaders/shaders.glsl");
    unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);

    /* set up vertex data (and buffer(s)) and configure vertex attributes */
    // ------------------------------------------------------------------
    float vertices[] = {
    	 // Position 			// color
         0.75f, 0.75f, 0.0f,	1.0f, 0.0f, 0.0f, 	1.0f, 1.0f,										// top right
        -0.75f, 0.75f, 0.0f, 	0.0f, 1.0f, 0.0f, 	0.0f, 1.0f,										// top left
        -0.75f,-0.75f, 0.0f, 	0.0f, 0.0f, 1.0f, 	0.0f, 0.0f,										// bottom left
         0.75f,-0.75f, 0.0f, 	0.0f, 0.0f, 0.0f, 	1.0f, 0.0f,										// bottom right
         0.5f,  0.0f, 0.0f, 	0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 	0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 	0.0f, 0.0f, 0.0f,
    };

//    float vertices[] = {
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//    };


    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        3, 2, 0     // second triangle
//		4, 5, 6
    };

//    glm::vec3 cubePositions[] = {
//        glm::vec3( 0.0f,  0.0f,  0.0f),
//        glm::vec3( 2.0f,  5.0f, -15.0f),
//        glm::vec3(-1.5f, -2.2f, -2.5f),
//        glm::vec3(-3.8f, -2.0f, -12.3f),
//        glm::vec3( 2.4f, -0.4f, -3.5f),
//        glm::vec3(-1.7f,  3.0f, -7.5f),
//        glm::vec3( 1.3f, -2.0f, -2.5f),
//        glm::vec3( 1.5f,  2.0f, -2.5f),
//        glm::vec3( 1.5f,  0.2f, -1.5f),
//        glm::vec3(-1.3f,  1.0f, -1.5f)
//    };

    // Coordinates for the textures, it has to relate the corner of the triangles with the position
    // of the texture.
//    float texCoords[] = {
//        0.0f, 0.0f,  // lower-left corner
//        1.0f, 0.0f,  // lower-right corner
//        0.5f, 1.0f   // top-center corner
//    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // this kind of object stores the info of the buffer and configuration of the attributes
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO); // Element Buffer Object
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Set up and creation the textures
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("texture/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("texture/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// ----------------------------------------------
	// Execute the program
	// ----------------------------------------------
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this call to draw in wireframe polygons.
    // be sure to activate the shader
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0); // set it manually
    glUniform1i(glGetUniformLocation(program, "texture2"), 1); // set it manually

	// ---------------------------
	// Coordinate Systems
	// ---------------------------

	// Model
	glm::mat4 model = glm::mat4(1.0f);
	unsigned int modelLoc = glGetUniformLocation(program, "model");// Assign any kind of matrix
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// View
	glm::mat4 view = glm::mat4(1.0f);
	unsigned int viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Camera
//	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // Camera position
//	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // Camera pointing at
//	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); // Camera direction (reverse) from the origin to the camera position.
//	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // unity vector in y axis
//	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraRight)); // calculate the right axis in the camera space
//	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); // Calculate the up axis in the camera space
//	glm::mat4 view;
//	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
//					   glm::vec3(0.0f, 0.0f, 0.0f),
//					   glm::vec3(0.0f, 1.0f, 0.0f));

	// Projection
	glm::mat4 projection = glm::mat4(1.0f);
	unsigned int projectionLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

//    glm::mat4 view = glm::mat4(1.0f);
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//
//    glm::mat4 projection = glm::mat4(1.0f);
//    projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
//
//   	unsigned int modelLoc = glGetUniformLocation(program, "model");
////    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//    unsigned int viewLoc = glGetUniformLocation(program, "view");
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//
//    unsigned int projectionLoc = glGetUniformLocation(program, "projection");
//    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));





    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(1.0f), glm::vec3(0.5f, 1.0f, 0.0f));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    	float currentFrame = glfwGetTime();
    	deltaTime = currentFrame -lastFrame;
    	lastFrame = currentFrame;

    	// Color - Textures
        glUniform1f(glGetUniformLocation(program, "visibleLevel"), visibleLevel);
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

//    	float camX = sin(glfwGetTime()) * radius;
//    	float camZ = cos(glfwGetTime()) * radius;
//    	view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f, 1.0, 0.0));
//    	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


        // Transformations
    	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    	projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));






//
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


//        glBindVertexArray(VBO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);

//        glBindVertexArray(VAO);
//        for(unsigned int i = 0; i < 10; i++)
//        {
//            model = glm::translate(model, cubePositions[i]);
//            float angle = 20.0f * i;
//            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }

        // glBindVertexArray(0); // no need to unbind it every time

        //glfwSetCursorPos(window, 200.0f, 55.0f);

        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }
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
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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

        /* The compilation didn't work out */
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /* This is like a program in c */
    /* First create the file */
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
    float cameraSpeed = 2.5f * deltaTime; // The speed is multiple to the time between frames to make it constant;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
    	visibleLevel += 0.1f;
    	if(visibleLevel > 1.0f) visibleLevel = 1.0f;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        visibleLevel -= 0.1f;
		if(visibleLevel < 0.0f) visibleLevel = 0.0f;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    	cameraPos += cameraSpeed * cameraFront;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    	cameraPos -= cameraSpeed * cameraFront;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}


