
// Include of the GLEW. always first than the glfw3
#include <GL/glew.h>
//Include of the GLFW
#include <GLFW/glfw3.h>

// Standard headers
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//
struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};

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
//
//
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

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* This has to be done after make a valid OpenGL rendering context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Creation of a Vertex Buffer */
    /* The points to settle down */
    float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };
    /* This works to define the number of vertex to use and do not repeat the info */
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    unsigned int buffer;
    glGenBuffers(1, &buffer); /* Creation of the buffer that returns a ID */
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Selecting the buffer created

    /* Store the buffer in the GPU memory statically */
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    /* Start with the atributes of the verteces */
    /* Enable the vertex attribute 0 */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    /* Creation of the index buffer object */
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    /* Store the buffer in the GPU memory */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    std::cout << "VERTEX:" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT:" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    unsigned int program = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(program);

    /* This vertex buffer will be called by a drawcall in the loping of the window */


    /* Loop until the user closes the window */
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && \
    		glfwWindowShouldClose(window) == 0 )
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Drawcall the last bind vertex buffer defined*/
        //glDrawArrays(GL_TRIANGLES, 0, 6); // this it the drawcall when you dont have any index buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // Always the index buffer need to be unsigned format


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}
//
//
//
//
