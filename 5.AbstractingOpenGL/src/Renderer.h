#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

// Include of the GLFW
#include <GLFW/glfw3.h>

// Include of the std messages to terminal
#include <iostream>


#ifdef MY_DEBUG
    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, 
                            GLsizei length, const char *message, const void *userParam);
#endif


#endif