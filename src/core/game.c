#include "game.h"
#include "game_objects.h"
#include "asset_utils.h"
#include "buffer.h"
#include "image.h"
#include "linmath.h"
#include "math_helper.h"
#include "platform_gl.h"
#include "platform_asset_utils.h"
#include "program.h"
#include "shader.h"
#include "texture.h"

static const float puck_height = 0.02f;
static const float mallet_height = 0.15f;

static Table table;
static Puck puck;
static Mallet red_mallet;
static Mallet blue_mallet;

static TextureProgram texture_program;
static ColorProgram color_program;

static mat4x4 projection_matrix;
static mat4x4 model_matrix;
static mat4x4 view_matrix;

static mat4x4 view_projection_matrix;
static mat4x4 model_view_projection_matrix;

static void position_table_in_scene();
static void position_object_in_scene(float x, float y, float z);

void on_surface_created() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	table = create_table(load_png_asset_into_texture("textures/air_hockey_surface.png"));

	vec4 puck_color = {0.8f, 0.8f, 1.0f, 1.0f};
	vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
	vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};

	puck = create_puck(0.06f, puck_height, 32, puck_color);
	red_mallet = create_mallet(0.08f, mallet_height, 32, red);
	blue_mallet = create_mallet(0.08f, mallet_height, 32, blue);

	texture_program = get_texture_program(build_program_from_assets("shaders/texture_shader.vsh", "shaders/texture_shader.fsh"));
	color_program = get_color_program(build_program_from_assets("shaders/color_shader.vsh", "shaders/color_shader.fsh"));
}

void on_surface_changed(int width, int height) {
	glViewport(0, 0, width, height);
	mat4x4_perspective(projection_matrix, 45, (float) width / (float) height, 1.0f, 10.0f);
	mat4x4_look_at(view_matrix, (vec3){0.0f, 1.2f, 2.2f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f});
}

void on_draw_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4x4_mul(view_projection_matrix, projection_matrix, view_matrix);

	position_table_in_scene();
    draw_table(&table, &texture_program, model_view_projection_matrix);

	position_object_in_scene(0.0f, mallet_height / 2.0f, -0.4f);
	draw_mallet(&red_mallet, &color_program, model_view_projection_matrix);

	position_object_in_scene(0.0f, mallet_height / 2.0f, 0.4f);
	draw_mallet(&blue_mallet, &color_program, model_view_projection_matrix);

	// Draw the puck.
	position_object_in_scene(0.0f, puck_height / 2.0f, 0.0f);
	draw_puck(&puck, &color_program, model_view_projection_matrix);
}

static void position_table_in_scene() {
	// The table is defined in terms of X & Y coordinates, so we rotate it
	// 90 degrees to lie flat on the XZ plane.
	mat4x4 rotated_model_matrix;
	mat4x4_identity(model_matrix);
	mat4x4_rotate_X(rotated_model_matrix, model_matrix, deg_to_radf(-90.0f));
	mat4x4_mul(model_view_projection_matrix, view_projection_matrix, rotated_model_matrix);
}

static void position_object_in_scene(float x, float y, float z) {
	mat4x4_identity(model_matrix);
	mat4x4_translate_in_place(model_matrix, x, y, z);
	mat4x4_mul(model_view_projection_matrix, view_projection_matrix, model_matrix);
}
