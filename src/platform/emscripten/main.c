#include <stdlib.h>
#include <stdio.h>
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
#include "game.h"

int init_gl();
void do_frame();
void shutdown_gl();

static const int width = 480, height = 800;

int main()
{
	if (init_gl() == GL_TRUE) {
		on_surface_created();
		on_surface_changed(width, height);
		emscripten_set_main_loop(do_frame, 0, 1);
	}
		
	shutdown_gl();

	return 0;
}

int init_gl()
{
	if (glfwInit() != GL_TRUE) {
		printf("glfwInit() failed\n");
		return GL_FALSE;
	}

	if (glfwOpenWindow(width, height, 8, 8, 8, 8, 16, 0, GLFW_WINDOW) != GL_TRUE) {
		printf("glfwOpenWindow() failed\n");
    	return GL_FALSE;
    }

    return GL_TRUE;
}

void do_frame()
{	
	on_draw_frame();
	glfwSwapBuffers();
}

void shutdown_gl()
{
	glfwTerminate();
}