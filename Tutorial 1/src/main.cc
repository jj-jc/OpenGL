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

/**
* Struct that stores the information of the code readen.
*/
struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

/**
* Returns the code of the shaders specified.
*
* @param  filepath the file in which the code is located
* @return          the stringstream
*/
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

/**
* This function creates an Object shaders depending on the type specified.
*
* @param  type   the type of the shader required.
* @param  source the code copied from the file.
* @return        the id of the object to reference later.
*/
static unsigned int CompileShader(unsigned int type, const std::string &source) {
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

/**
* This function creates the program that runs inside the GPU.
*
* @param  vertexShader   the vertexShader that want to use.
* @param  fragmentShader the fragmentShader that want to use.
* @return program        the program created to run inside the GPU..
*/
static unsigned int CreateProgram(const std::string& vertexShader, const std::string& fragmentShader) {
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

    /* It is possible to detach the the shaders from the program once linked
     * but it is usefull to mantein to debug (it can bracets with
     */
    return program;
}

int main(void)
{
	// Some characteristics of the Window
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para hacer feliz a MacOS ; Aunque no debería ser necesaria
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No queremos el viejo OpenGL

	GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Tutorial", NULL, NULL);
    if (!window)
    {
    	std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Make the window's context
    glfwSwapInterval(1); // activate v-sync. To synchronize the frames of the screen

    /* This has to be done after make a valid OpenGL rendering context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Creation of a Vertex Buffer */
    /* The points to settle down */
//    float positions[] = {
//        -0.5f, -0.5f, // 0
//         0.5f, -0.5f, // 1
//         0.5f,  0.5f, // 2
//        -0.5f,  0.5f  // 3
//    };

    float little_positions[] = {
        -0.25f, -0.25f, // 0
         0.25f, -0.25f, // 1
         0.25f,  0.25f, // 2
        -0.25f,  0.25f  // 3
    };
    /* This works to define the number of vertex to use and do not repeat the info */
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    GLuint vertexArrayID;
    glGenBuffers(1, &vertexArrayID); /* Creation of the buffer that returns a ID */
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayID); // Selecting the buffer created

    /* Store the buffer in the GPU memory statically */
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), little_positions, GL_STATIC_DRAW);

    /* Start with the attributes of the vertices */
    /* Enable the vertex attribute 0 */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    /* Creation of the index buffer object */
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // Element array buffer
    /* Store the buffer in the GPU memory */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ShaderProgramSource source = ParseShader("Shaders/shaders.glsl"); // The path is related with the workspace
    std::cout << "VERTEX:" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT:" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int program = CreateProgram(source.VertexSource, source.FragmentSource);
    glUseProgram(program);

    // Get the location of the uniform declared in the fragment shader
    GLint location = glGetUniformLocation(program, "u_Color");
    glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f); // set a value to the uniform get before


    float r = 0.0f;
    float increment = 0.05f;

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    /* Loop until the user closes the window */
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && \
    		glfwWindowShouldClose(window) == 0 )
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw-call the last bind vertex buffer defined*/
        //glDrawArrays(GL_TRIANGLES, 0, 6); // this it the draw-call when you don't have any index buffer

        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        //Draw the elements defined with bind
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // Always the index buffer need to be unsigned format

        if (r > 1.0f)
        	increment = -0.05f;
        else if (r < 0.0f)
        	increment = 0.005f;

        r += increment;


        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }


    // Cleanup VBO (Vertex Array Object)
    glDeleteBuffers(1,&vertexArrayID);
    glDeleteProgram(program);

    glfwTerminate(); // Close OpenGlL window and terminate GLFW

    return 0;
}

