#include "game.h"
#include "game_objects.h"
#include "asset_utils.h"
#include "buffer.h"
#include "geometry.h"
#include "image.h"
#include "linmath.h"
#include "math_helper.h"
#include "platform_gl.h"
#include "platform_asset_utils.h"
#include "program.h"
#include "shader.h"
#include "texture.h"

static const float puck_height = 0.02f;
static const float puck_radius = 0.06f;
static const float mallet_height = 0.15f;
static const float mallet_radius = 0.08f;

static const float left_bound = -0.5f;
static const float right_bound = 0.5f;
static const float far_bound = -0.8f;
static const float near_bound = 0.8f;

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
static mat4x4 inverted_view_projection_matrix;

static int mallet_pressed;
static vec3 blue_mallet_position;
static vec3 previous_blue_mallet_position;
static vec3 puck_position;
static vec3 puck_vector;

static Ray convert_normalized_2D_point_to_ray(float normalized_x, float normalized_y);
static void divide_by_w(vec4 vector);
static float clamp(float value, float min, float max);
static void position_table_in_scene();
static void position_object_in_scene(float x, float y, float z);

void on_touch_press(float normalized_x, float normalized_y) {
	Ray ray = convert_normalized_2D_point_to_ray(normalized_x, normalized_y);

	// Now test if this ray intersects with the mallet by creating a
	// bounding sphere that wraps the mallet.
	Sphere mallet_bounding_sphere = (Sphere) {
	   {blue_mallet_position[0],
		blue_mallet_position[1],
		blue_mallet_position[2]},
	mallet_height / 2.0f};

	// If the ray intersects (if the user touched a part of the screen that
	// intersects the mallet's bounding sphere), then set malletPressed =
	// true.
	mallet_pressed = sphere_intersects_ray(mallet_bounding_sphere, ray);
}

static Ray convert_normalized_2D_point_to_ray(float normalized_x, float normalized_y) {
	// We'll convert these normalized device coordinates into world-space
	// coordinates. We'll pick a point on the near and far planes, and draw a
	// line between them. To do this transform, we need to first multiply by
	// the inverse matrix, and then we need to undo the perspective divide.
	vec4 near_point_ndc = {normalized_x, normalized_y, -1, 1};
	vec4 far_point_ndc = {normalized_x, normalized_y,  1, 1};

    vec4 near_point_world, far_point_world;
    mat4x4_mul_vec4(near_point_world, inverted_view_projection_matrix, near_point_ndc);
    mat4x4_mul_vec4(far_point_world, inverted_view_projection_matrix, far_point_ndc);

	// Why are we dividing by W? We multiplied our vector by an inverse
	// matrix, so the W value that we end up is actually the *inverse* of
	// what the projection matrix would create. By dividing all 3 components
	// by W, we effectively undo the hardware perspective divide.
    divide_by_w(near_point_world);
    divide_by_w(far_point_world);

	// We don't care about the W value anymore, because our points are now
	// in world coordinates.
	vec3 near_point_ray = {near_point_world[0], near_point_world[1], near_point_world[2]};
	vec3 far_point_ray = {far_point_world[0], far_point_world[1], far_point_world[2]};
	vec3 vector_between;
	vec3_sub(vector_between, far_point_ray, near_point_ray);
	return (Ray) {
		{near_point_ray[0], near_point_ray[1], near_point_ray[2]},
		{vector_between[0], vector_between[1], vector_between[2]}};
}

static void divide_by_w(vec4 vector) {
	vector[0] /= vector[3];
	vector[1] /= vector[3];
	vector[2] /= vector[3];
}

void on_touch_drag(float normalized_x, float normalized_y) {
	if (mallet_pressed == 0)
		return;

	Ray ray = convert_normalized_2D_point_to_ray(normalized_x, normalized_y);
	// Define a plane representing our air hockey table.
	Plane plane = (Plane) {{0, 0, 0}, {0, 1, 0}};

	// Find out where the touched point intersects the plane
	// representing our table. We'll move the mallet along this plane.
	vec3 touched_point;
	ray_intersection_point(touched_point, ray, plane);

	memcpy(previous_blue_mallet_position, blue_mallet_position, sizeof(blue_mallet_position));

	// Clamp to bounds
	blue_mallet_position[0] = clamp(touched_point[0], left_bound + mallet_radius, right_bound - mallet_radius);
	blue_mallet_position[1] = mallet_height / 2.0f;
	blue_mallet_position[2] = clamp(touched_point[2], 0.0f + mallet_radius, near_bound - mallet_radius);

	// Now test if mallet has struck the puck.
	vec3 mallet_to_puck;
	vec3_sub(mallet_to_puck, puck_position, blue_mallet_position);
	float distance = vec3_len(mallet_to_puck);

	if (distance < (puck_radius + mallet_radius)) {
		// The mallet has struck the puck. Now send the puck flying
		// based on the mallet velocity.
		vec3_sub(puck_vector, blue_mallet_position, previous_blue_mallet_position);
	}
}

static float clamp(float value, float min, float max) {
	return fmin(max, fmax(value, min));
}

void on_surface_created() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	table = create_table(load_png_asset_into_texture("textures/air_hockey_surface.png"));

	vec4 puck_color = {0.8f, 0.8f, 1.0f, 1.0f};
	vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
	vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};

	puck = create_puck(puck_radius, puck_height, 32, puck_color);
	red_mallet = create_mallet(mallet_radius, mallet_height, 32, red);
	blue_mallet = create_mallet(mallet_radius, mallet_height, 32, blue);

	blue_mallet_position[0] = 0;
	blue_mallet_position[1] = mallet_height / 2.0f;
	blue_mallet_position[2] = 0.4f;
	puck_position[0] = 0;
	puck_position[1] = puck_height / 2.0f;
	puck_position[2] = 0;
	puck_vector[0] = 0;
	puck_vector[1] = 0;
	puck_vector[2] = 0;

	texture_program = get_texture_program(build_program_from_assets("shaders/texture_shader.vsh", "shaders/texture_shader.fsh"));
	color_program = get_color_program(build_program_from_assets("shaders/color_shader.vsh", "shaders/color_shader.fsh"));
}

void on_surface_changed(int width, int height) {
	glViewport(0, 0, width, height);
	// 45 degrees, in radians
	mat4x4_perspective(projection_matrix, 0.785398163, (float) width / (float) height, 1.0f, 10.0f);
	mat4x4_look_at(view_matrix, (vec3){0.0f, 1.2f, 2.2f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f});
}

void on_draw_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Translate the puck by its vector
	vec3_add(puck_position, puck_position, puck_vector);

	// If the puck struck a side, reflect it off that side.
	if (puck_position[0] < left_bound + puck_radius
	 || puck_position[0] > right_bound - puck_radius) {
		puck_vector[0] = -puck_vector[0];
		vec3_scale(puck_vector, puck_vector, 0.9f);
	}
	if (puck_position[2] < far_bound + puck_radius
	 || puck_position[2] > near_bound - puck_radius) {
		puck_vector[2] = -puck_vector[2];
		vec3_scale(puck_vector, puck_vector, 0.9f);
	}

	// Clamp the puck position.
	puck_position[0] = clamp(puck_position[0], left_bound + puck_radius, right_bound - puck_radius);
	puck_position[2] = clamp(puck_position[2], far_bound + puck_radius, near_bound - puck_radius);

	// Friction factor
	vec3_scale(puck_vector, puck_vector, 0.99f);

    mat4x4_mul(view_projection_matrix, projection_matrix, view_matrix);
    mat4x4_invert(inverted_view_projection_matrix, view_projection_matrix);

	position_table_in_scene();
    draw_table(&table, &texture_program, model_view_projection_matrix);

	position_object_in_scene(0.0f, mallet_height / 2.0f, -0.4f);
	draw_mallet(&red_mallet, &color_program, model_view_projection_matrix);

	position_object_in_scene(blue_mallet_position[0], blue_mallet_position[1], blue_mallet_position[2]);
	draw_mallet(&blue_mallet, &color_program, model_view_projection_matrix);

	// Draw the puck.
	position_object_in_scene(puck_position[0], puck_position[1], puck_position[2]);
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
