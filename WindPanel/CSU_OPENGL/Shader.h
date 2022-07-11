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
        mutable std::unordered_map<std::string, int> m_UniformLocationCache; // this is mutable to be able to change its value by the getUniformLocation function
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        void use() const;      
        void unbind() const;

        // TODO: do it more flexible instead of implement all the possibilities
        // set uniforms
        void setUniform1i(const std::string& name, int value); 
        void setUniform1f(const std::string& name, float value);
        void setUniform3f(const std::string& name, glm::vec3 floats);
        void setUniform4f(const std::string& name, glm::vec4 floats);
        void setUniformMatrix4fv(const std::string& name, const glm::mat4 matrix);
        static std::string getShaderSource(const char* shaderFile);
    private:
        int getUniformLocation(const std::string& name) const; // this is conts because doesn't change the uniform information at all.

};

#endif