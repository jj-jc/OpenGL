#include "Shader.h"

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
{
    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *source = (const GLchar *)vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        // FIXME: not use std cout to show errors
        // Use the infoLog as you see fit.
        std::cout << "Compiling error of the vertex Shader" << std::endl;
        // In this simple program, we'll just leave
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar *)fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        //FIXME: write the log with the error
        // Use the infoLog as you see fit.
        std::cout << "Compiling error of the fragment Shader" << std::endl;
        
        // In this simple program, we'll just leave
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    this->m_RendererID = glCreateProgram();
    GLuint program = this->m_RendererID;

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // FIXME: write the log with the error
        // Use the infoLog as you see fit.
        std::cout << "linking error of the: " << m_RendererID << " Shader program" << std::endl;
        // In this simple program, we'll just leave
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::use() const
{
    glUseProgram(this->m_RendererID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform1i(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name.c_str()), value);
}

void Shader::setUniform1f(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name.c_str()), value);
}

void Shader::setUniform3f(const std::string& name, glm::vec3 floats)
{
    glUniform3f(getUniformLocation(name.c_str()), floats.x, floats.y, floats.z);
}

void Shader::setUniform4f(const std::string& name, glm::vec4 floats)
{
    glUniform4f(getUniformLocation(name.c_str()), floats.x, floats.y, floats.z, floats.w);
}

void Shader::setUniformMatrix4fv(const std::string& name, const glm::mat4 matrix)
{
    glUniformMatrix4fv(getUniformLocation(name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}


int Shader::getUniformLocation(const std::string& name) const
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    //FIXME: use log no std::cout
    if(location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exits!" << std::endl;
    
    m_UniformLocationCache[name] = location;

    return location;
}

std::string Shader::getShaderSource(const char* shaderFile)
{
    std::string shaderSource;
    std::ifstream fparser;
        // ensure ifstream object can throw exceptions:
    fparser.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        fparser.open(shaderFile);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << fparser.rdbuf();
        // close file handlers
        fparser.close();
        // convert stream into string
        shaderSource = shaderStream.str();
    }
    catch(const std::exception& e)
    {   // FIXME: write the log insted of cout
        std::cout << e.what() << std::endl;
    }

    return shaderSource;
}

