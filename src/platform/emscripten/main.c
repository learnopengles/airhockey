#include <stdlib.h>
#include <stdio.h>
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
#include "game.h"

static int init_gl();
static void do_frame();
static void handle_input();
static void shutdown_gl();

static const int width = 480, height = 800;
int is_dragging;

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

static int init_gl()
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

static void do_frame()
{	
	handle_input();
	on_draw_frame();
	glfwSwapBuffers();
}

static void handle_input()
{
	glfwPollEvents();
	const int left_mouse_button_state = glfwGetMouseButton(GLFW_MOUSE_BUTTON_1);
	if (left_mouse_button_state == GLFW_PRESS) {
		int x_pos, y_pos;
		glfwGetMousePos(&x_pos, &y_pos);
		const float normalized_x = ((float)x_pos / (float) width) * 2.f - 1.f;
	    const float normalized_y = -(((float)y_pos / (float) height) * 2.f - 1.f);

		if (is_dragging == 0) {
			is_dragging = 1;			
			on_touch_press(normalized_x, normalized_y);
		} else {
			on_touch_drag(normalized_x, normalized_y);
		}
	} else {
		is_dragging = 0;
	}
}

static void shutdown_gl()
{
	glfwTerminate();
}