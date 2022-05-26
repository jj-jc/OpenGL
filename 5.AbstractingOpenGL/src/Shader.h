#ifndef _SHADER_H_
#define _SHADER_H_

// OpenGL API
#include <GL/glew.h>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
//FIXME: delete the iostream include
#include <iostream>
// OpenGL Mathematics and is a header-only library
#include <glm/glm.hpp>
// some cool staf
#include <unordered_map>

class Shader
{
    private:
        std::string m_FilePath;
        unsigned int m_RendererID;
        std::unordered_map<std::string, int> m_UniformLocationCache;
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void bind() const;      // its not bind really, it should be program use but to mantein it coherence with the other definitions
        void unbind() const;

        // TODO: do it more flexible instead of implement all the possibilities
        // set uniforms
        void setUniform4f(const std::string& name, glm::vec4 floats);
        static std::string getShaderSource(const char* shaderFile);
    private:
        int getUniformLocation(const std::string& name);

};

#endif