#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <cstdio>
#include <limits>
#include <chrono>
#include <thread>
#include <mutex>

#include "CSU_OPENGL/Shader.h"

//#define GL33
//#define FULLSCREEN


static const int width = 640;   
static const int height = 360;

GLFWwindow* window;
GLFWwindow* window_slave;
GLuint fb[2] = {std::numeric_limits<GLuint>::max(), std::numeric_limits<GLuint>::max()}; //framebuffers
GLuint rb[2] = {std::numeric_limits<GLuint>::max(), std::numeric_limits<GLuint>::max()}; //renderbuffers, color and depth this are shared among the contexts

bool threadShouldRun = true;
bool isFBOdirty = true; //true when last frame was displayed, false
						//when just updated and not yet displayed
bool isFBOready = false; //set by worker thread when initialized
bool isFBOsetupOnce = false; //set by main thread when initialized

std::timed_mutex mutexGL;

// Changes to draw a simple triangle
/* Definitions of objects */
float positions[] = {
	// positions         // colors
		0.0f, -0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		2.0f,  2.0f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  2.0f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};
GLuint vbo;


static bool checkFrameBuffer(GLuint fbuffer)
{
	bool isFB = glIsFramebuffer(fbuffer);
	bool isCA = glIsRenderbuffer(rb[0]);
	bool isDSA = glIsRenderbuffer(rb[1]);
	bool isComplete = false;
	if(isFB){
		glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);		
		isComplete = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}
	
/*
	printf("Is fb a framebuffer? %s\n", isFB ? "[yes]" : "[no]");
	printf("Is rb[0] a color renderbuffer? %s\n", isCA ? "[yes]" : "[no]");
	printf("Is rv[1] a depth stencil renderbuffer? %s\n", isDSA ? "[yes]" : "[no]");
	printf("Is fb framebuffer-complete? %s\n", isComplete ? "[yes]" : "[no]");
*/
	return isFB && isCA &&isDSA && isComplete;	
}

/* worker thread creates its own FBO to render to, as well as two renderbuffers.
The renderbuffers are also used by a separate FBO in main()
fb[0] is owned by main, and fb[1] is owned by worker thread
*/
static bool createFrameBuffer() //for worker thread
{
	bool ret;
	
	glGenFramebuffers(1, &fb[1]);
	glBindFramebuffer(GL_FRAMEBUFFER, fb[1]);
	glGenRenderbuffers(2, rb);
	glBindRenderbuffer(GL_RENDERBUFFER, rb[0]);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 2, GL_RGBA8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, rb[1]);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 2, GL_DEPTH24_STENCIL8, width, height);

	glBindRenderbuffer(GL_RENDERBUFFER, rb[0]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb[0]); // attach the renderbuffer as a color buffer
	glBindRenderbuffer(GL_RENDERBUFFER, rb[1]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb[1]); // attach the renderbuffer as a depth buffer

	glFlush();

	if(!(ret = checkFrameBuffer(fb[1]))){
		glDeleteRenderbuffers(2, rb);
		glDeleteFramebuffers(1,  &fb[1]);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	return ret;	
}

/* If worker thread is finished initializing renderbuffers, reuse them in a FBO for main.
fb[0] is owned by main, and fb[1] is owned by worker thread */
static void createFrameBufferMain()
{
	while(!mutexGL.try_lock_for(std::chrono::seconds(1))){
		return;
	}
	
	if(isFBOready){ //is other thread finished setting up FBO?
		if(glIsRenderbuffer(rb[0]) && glIsRenderbuffer(rb[1])){
			glBindFramebuffer(GL_FRAMEBUFFER, fb[0]);	
			glBindRenderbuffer(GL_RENDERBUFFER, rb[0]);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb[0]);
			glBindRenderbuffer(GL_RENDERBUFFER, rb[1]);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb[1]);
			isFBOsetupOnce = true;
		}
	}
	mutexGL.unlock();
}

/* Used in main to copy from fbo into the real framebuffer.
fb[0] FBO owned by main reuses renderbuffers from worker thread.
The contents of those renderbuffers are then copied into the
default framebuffer by this function. */
static void copyFrameBuffer(GLuint fbuffer)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbuffer); // bind to the read framebuffer with the info of fbuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // pointe to a empty draw framebuffer
	glBlitFramebuffer(0, 0, width, height,
					  0, 0, width, height,
					  GL_COLOR_BUFFER_BIT,
					  GL_NEAREST); // copy a block of pixels from the read framebuffer to the draw framebuffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); // bind to an empty read framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // bind to an empty draw framebuffer
}

static GLFWwindow* initWindow(GLFWwindow* shared, bool visible)
{
	GLFWwindow* win;
#ifdef GL33
 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif
	if(visible)
		glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	else 
		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	
#ifdef FULLSCREEN
	GLFWmonitor* monitor = 0;
	if(visible) //Don't create fullscreen window for offscreen contexts
		monitor = glfwGetPrimaryMonitor();	
    win = glfwCreateWindow(width, height, "Optimus example", monitor, shared);
#else
    win = glfwCreateWindow(width, height, "Optimus example", 0, shared);
#endif
	return win;	
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_ENTER)
		&& action == GLFW_PRESS){		
        glfwSetWindowShouldClose(window, GL_TRUE);
	}
}



/********************************************** TEST *********************************/
static void worker_thread()
{
	glfwMakeContextCurrent(window_slave);
	//create new shared framebuffer object
	mutexGL.lock();
	createFrameBuffer();
	isFBOready = true;
	mutexGL.unlock();
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // Vertex Attribute position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // Vertex Attribute colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	
	Shader ourShader(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/2.Shaders/res/shaders/triangle.vs"), Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/2.Shaders/res/shaders/triangle.fs"));
	
	for(;;){
		mutexGL.lock();
		if(!threadShouldRun){
			mutexGL.unlock();
			break;
		}
		if(isFBOdirty){			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, fb[1]);
			float r = (float)rand() / (float)RAND_MAX;		
			// drawcall for the second context
			ourShader.use();

			glDrawArrays(GL_TRIANGLES, 0, 3);
			// glClearColor(r,r,r,1.0f);
			glClearColor(1.0f,0.0f,0.0f,0.0f);
			glFlush();
			isFBOdirty = false;
		}
		mutexGL.unlock();
	}
	printf("Exiting thread..\n");
	return;	
}

int main(int argc, char* argv[])
{	
	if(!glfwInit()){
		printf("Failed to initialize glfw\n");		
		return 0;
	}
	//main window
	window = initWindow(0, true);
	//window used by second thread
	window_slave = initWindow(window, false); // create a window and a context shared with the main window

	if(!window || !window_slave){
		glfwTerminate();
		printf("Failed to create glfw windows\n");
		return 0;		
	}
	glfwSetKeyCallback(window, key_callback);	
	glfwMakeContextCurrent(window);
	
	if(glewInit()){
		printf("Failed to init GLEW\n");
		glfwDestroyWindow(window);
		glfwDestroyWindow(window_slave);
		glfwTerminate();		
		return 0;
	}

	
	glGenFramebuffers(1, &fb[0]);
	// glViewport(0, 0, width, height);

    // GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // Vertex Attribute position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // Vertex Attribute colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
 
	std::thread gl_thread(worker_thread);

	Shader ourShader(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/2.Shaders/res/shaders/triangle.vs"), Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/2.Shaders/res/shaders/triangle.fs"));

	while(!glfwWindowShouldClose(window)){
		glfwPollEvents(); //get key input
		if(!isFBOsetupOnce){
			createFrameBufferMain(); //isFBOsetupOnce = true when FBO can be used (just one time)
		} else {
			if(checkFrameBuffer(fb[0])){
				if(!mutexGL.try_lock_for(std::chrono::seconds(1)))
					continue;			
				if(!isFBOdirty){					
					copyFrameBuffer(fb[0]);		
					ourShader.use();
					// ourShader.use();
					// glDrawArrays(GL_TRIANGLES, 0, 3);
					glfwSwapBuffers(window);
					isFBOdirty = true;
					ourShader.unbind();
					//printf("Framebuffer OK\n");			
				} else {
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					// ourShader.use();
        			// ourShader.setUniform1i("someUniform", 1.0f);
					ourShader.use();
        			glDrawArrays(GL_TRIANGLES, 0, 3);
					glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					ourShader.unbind();
				}
				mutexGL.unlock();
			} else {
				printf("Framebuffer not ready!\n");
				GLenum e = glGetError();
				printf("OpenGL error: %X\n", e);			
			}
		}		
    }
    threadShouldRun = false; //other thread only reads this
    gl_thread.join();
    glfwDestroyWindow(window);
    glfwDestroyWindow(window_slave);
    glfwTerminate();
    return 0;
}

