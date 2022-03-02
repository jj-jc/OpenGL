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
#include <stb_image.h> // This manage the conversion of textures in bynaries.

// Include own header files
#include "Shader.hpp"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

static float visibleLevel = 0.1f;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
static void error_callback(int error, const char* description);
static ShaderProgramSource ParseShader(const std::string& filepath);
static unsigned int CompileShader(unsigned int type, const std::string& source);
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);




int main(void)
{
	std::cout << "Texture example:" << std::endl;

	// ---------------------------------------------------------------------
	// Setting up the context in GLFW and glew
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	/* Initialize the library */
    if (!glfwInit())
    	exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Texture example", NULL, NULL);
    if (!window)
    {
    	std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window); // Make the window's context
    glfwSwapInterval(1); // activate v-sync. To synchronize the frames of the screen
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /* This has to be done after make a valid OpenGL rendering context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    glfwSetKeyCallback(window, key_callback);
    //------------------------------------------------------------------------

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



    /* Set up and creation the shader program */
    // ------------------------------------------------------------------
    ShaderProgramSource source = ParseShader("Shaders/shaders.glsl");
    /*
    std::cout << "VERTEX:" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT:" << std::endl;
    std::cout << source.FragmentSource << std::endl;
    */
    unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);

    /* set up vertex data (and buffer(s)) and configure vertex attributes */
    // ------------------------------------------------------------------
    float vertices[] = {
    	 // Position 			// color
         0.75f, 0.75f, 0.0f,	1.0f, 0.0f, 0.0f, 	1.0f, 1.0f,										// top right
        -0.75f, 0.75f, 0.0f, 	0.0f, 1.0f, 0.0f, 	0.0f, 1.0f,										// top left
        -0.75f, 0.0f, 0.0f, 	0.0f, 0.0f, 1.0f, 	0.0f, 0.0f,										// bottom left
         0.75f,  0.0f, 0.0f, 	0.0f, 0.0f, 0.0f, 	1.0f, 0.0f,										// bottom right
         0.5f,  0.0f, 0.0f, 	0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 	0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 	0.0f, 0.0f, 0.0f,
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        3, 2, 0     // second triangle
//		4, 5, 6
    };

    // coordenates for the textures, it has to relate the corner of the triangles with the position
    // of the texture.
//    float texCoords[] = {
//        0.0f, 0.0f,  // lower-left corner
//        1.0f, 0.0f,  // lower-right corner
//        0.5f, 1.0f   // top-center corner
//    };

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // be sure to activate the shader
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0); // set it manually
    glUniform1i(glGetUniformLocation(program, "texture2"), 1); // set it manually

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT);



    	float timeValue = glfwGetTime();
    	float greenValue = sin(timeValue) / 2.0f + 0.5f;
    	int vertexColorLocation = glGetUniformLocation(program, "colorTriangle");

        // draw our first triangle
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glUniform1f(glGetUniformLocation(program, "visibleLevel"), visibleLevel);
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
    	visibleLevel += 0.1f;
    	if(visibleLevel > 1.0f) visibleLevel = 1.0f;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        visibleLevel -= 0.1f;
		if(visibleLevel < 0.0f) visibleLevel = 0.0f;
    }
}

