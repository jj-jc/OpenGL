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


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    	std::cout << "Esc key pressed" << std::endl;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        std::cout << "Space key pressed" << std::endl;
    	std::cout << "The value of the time is: " << glfwGetTime() << std::endl;
    }
    else if (key == GLFW_KEY_X && action == GLFW_PRESS){
    	std::cout << "X key pressed" << std::endl;
    	const int scancode = glfwGetKeyScancode(GLFW_KEY_X);
    	std::cout << "The scancode is: " << scancode << std::endl;
    	std::cout << "X key pressed" << std::endl;
    }

}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	std::cout << "This is an character callback" << std::endl;
	std::cout << "This is the value: " << (char)codepoint << std::endl;
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "This is the position of the mouse:"
			<< " x = " << xpos
			<< " y = " << xpos
			<< std::endl;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
    	std::cout << "The mouse have been pressed" << std::endl;
    	double xpos, ypos;
    	glfwGetCursorPos(window, &xpos, &ypos);
    	std::cout << "This is the position of the mouse:"
    			<< " x = " << xpos
				<< " y = " << ypos
				<< std::endl;
    }
}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	/* Initialize the library */
    if (!glfwInit())
    	exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Tutorial", NULL, NULL);
    if (!window)
    {
    	std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    //glfwSetCursorPosCallback(window, cursor_position_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window); // Make the window's context
    glfwSwapInterval(1); // activate v-sync. To synchronize the frames of the screen


    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    /* This has to be done after make a valid OpenGL rendering context */
    if (glewInit() != GLEW_OK)
        std::cout << "Error with the glewInit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Creation of a Vertex Buffer */
    /* The points to settle down */

    unsigned char pixels[16 * 16 * 4];
    memset(pixels, 0xff, sizeof(pixels));

    GLFWimage image;
    image.width = 16;
    image.height = 16;
    image.pixels = pixels;
    GLFWcursor* cursor = glfwCreateCursor(&image, 0.5f, 0.5f);
    if(cursor == NULL)
    	std::cout << "Cursor creator has failed" << std::endl;
    else
    	std::cout << "Cursor has been created" << std::endl;
    glfwSetCursor(window, cursor);
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
		glClearColor (0.0, 0.0, 0.0, 0.0);
    	glClear(GL_COLOR_BUFFER_BIT);
    	glColor3f (1.0, 1.0, 1.0);
    	glBegin(GL_POLYGON);
    		glVertex3f (0.25, 0.25, 0.0);
    		glVertex3f (0.75, 0.25, 0.0);
    		glVertex3f (0.75, 0.75, 0.0);
    		glVertex3f (0.25, 0.75, 0.0);
		glEnd();

		glColor3f(0.5f,0.5f,1.0f);              // Set The Color To Blue One Time Only
		glBegin(GL_QUADS);                  // Start Drawing Quads
			glVertex3f(-0.20f,-0.20f, 0.00f);          // Left And Up 1 Unit (Top Left)
			glVertex3f( 0.20f,-0.20f, 0.00f);          // Right And Up 1 Unit (Top Right)
			glVertex3f( 0.20f, 0.20f, 0.00f);          // Right And Down One Unit (Bottom Right)
			glVertex3f(-0.20f, 0.20f, 0.00f);          // Left And Down One Unit (Bottom Left)
		glEnd();


        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }

    glfwDestroyWindow(window);


    glfwTerminate(); // Close OpenGlL window and terminate GLFW
    exit(EXIT_SUCCESS);
}

