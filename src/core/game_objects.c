#include "game_objects.h"
#include "buffer.h"
#include "platform_gl.h"
#include "program.h"
#include "linmath.h"
#include <math.h>

// Triangle fan
// position X, Y, texture S, T
static const float table_data[] = { 0.0f,  0.0f, 0.5f, 0.5f,
        						   -0.5f, -0.8f, 0.0f, 0.9f,
        						   	0.5f, -0.8f, 1.0f, 0.9f,
        						   	0.5f,  0.8f, 1.0f, 0.1f,
        						   -0.5f,  0.8f, 0.0f, 0.1f,
        						   -0.5f, -0.8f, 0.0f, 0.9f};

Table create_table(GLuint texture) {
	return (Table) {texture, create_vbo(sizeof(table_data), table_data, GL_STATIC_DRAW)};
}

void draw_table(const Table* table, const TextureProgram* texture_program, mat4x4 m)
{
	glUseProgram(texture_program->program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, table->texture);
	glUniformMatrix4fv(texture_program->u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)m);
	glUniform1i(texture_program->u_texture_unit_location, 0);

	glBindBuffer(GL_ARRAY_BUFFER, table->buffer);
	glVertexAttribPointer(texture_program->a_position_location, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), BUFFER_OFFSET(0));
	glVertexAttribPointer(texture_program->a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), BUFFER_OFFSET(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(texture_program->a_position_location);
	glEnableVertexAttribArray(texture_program->a_texture_coordinates_location);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static inline int size_of_circle_in_vertices(int num_points) {
	return 1 + (num_points + 1);
}

static inline int size_of_open_cylinder_in_vertices(int num_points) {
	return (num_points + 1) * 2;
}

static inline int gen_circle(float* out, int offset, float center_x, float center_y, float center_z, float radius, int num_points)
{
	out[offset++] = center_x;
	out[offset++] = center_y;
	out[offset++] = center_z;

	int i;
	for (i = 0; i <= num_points; ++i) {
		float angle_in_radians = ((float) i / (float) num_points) * ((float) M_PI * 2.0f);
		out[offset++] = center_x + radius * cos(angle_in_radians);
		out[offset++] = center_y;
		out[offset++] = center_z + radius * sin(angle_in_radians);
	}

	return offset;
}

static inline int gen_cylinder(float* out, int offset, float center_x, float center_y, float center_z, float height, float radius, int num_points)
{
	const float y_start = center_y - (height / 2.0f);
	const float y_end = center_y + (height / 2.0f);

	int i;
	for (i = 0; i <= num_points; i++) {
		float angle_in_radians = ((float) i / (float) num_points) * ((float) M_PI * 2.0f);

		float x_position = center_x + radius * cos(angle_in_radians);
		float z_position = center_z + radius * sin(angle_in_radians);

		out[offset++] = x_position;
		out[offset++] = y_start;
		out[offset++] = z_position;

		out[offset++] = x_position;
		out[offset++] = y_end;
		out[offset++] = z_position;
	}

	return offset;
}

Puck create_puck(float radius, float height, int num_points, vec4 color)
{
	float data[(size_of_circle_in_vertices(num_points) + size_of_open_cylinder_in_vertices(num_points)) * 3];

	int offset = gen_circle(data, 0, 0.0f, height / 2.0f, 0.0f, radius, num_points);
	gen_cylinder(data, offset, 0.0f, 0.0f, 0.0f, height, radius, num_points);

	return (Puck) {{color[0], color[1], color[2], color[3]},
				   create_vbo(sizeof(data), data, GL_STATIC_DRAW),
				   num_points};
}

void draw_puck(const Puck* puck, const ColorProgram* color_program, mat4x4 m)
{
	glUseProgram(color_program->program);

	glUniformMatrix4fv(color_program->u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)m);
	glUniform4fv(color_program->u_color_location, 1, puck->color);

	glBindBuffer(GL_ARRAY_BUFFER, puck->buffer);
	glVertexAttribPointer(color_program->a_position_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(color_program->a_position_location);

	int circle_vertex_count = size_of_circle_in_vertices(puck->num_points);
	int cylinder_vertex_count = size_of_open_cylinder_in_vertices(puck->num_points);

	glDrawArrays(GL_TRIANGLE_FAN, 0, circle_vertex_count);
	glDrawArrays(GL_TRIANGLE_STRIP, circle_vertex_count, cylinder_vertex_count);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mallet create_mallet(float radius, float height, int num_points, vec4 color)
{
	float data[(size_of_circle_in_vertices(num_points) * 2 + size_of_open_cylinder_in_vertices(num_points) * 2) * 3];

	float base_height = height * 0.25f;
	float handle_height = height * 0.75f;
	float handle_radius = radius / 3.0f;

	int offset = gen_circle(data, 0, 0.0f, -base_height, 0.0f, radius, num_points);
	offset = gen_circle(data, offset, 0.0f, height * 0.5f, 0.0f, handle_radius, num_points);
	offset = gen_cylinder(data, offset, 0.0f, -base_height - base_height / 2.0f, 0.0f, base_height, radius, num_points);
	gen_cylinder(data, offset, 0.0f, height * 0.5f - handle_height / 2.0f, 0.0f, handle_height, handle_radius, num_points);

	return (Mallet) {{color[0], color[1], color[2], color[3]},
					 create_vbo(sizeof(data), data, GL_STATIC_DRAW),
				     num_points};
}

void draw_mallet(const Mallet* mallet, const ColorProgram* color_program, mat4x4 m)
{
	glUseProgram(color_program->program);

	glUniformMatrix4fv(color_program->u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)m);
	glUniform4fv(color_program->u_color_location, 1, mallet->color);

	glBindBuffer(GL_ARRAY_BUFFER, mallet->buffer);
	glVertexAttribPointer(color_program->a_position_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(color_program->a_position_location);

	int circle_vertex_count = size_of_circle_in_vertices(mallet->num_points);
	int cylinder_vertex_count = size_of_open_cylinder_in_vertices(mallet->num_points);
	int start_vertex = 0;

	glDrawArrays(GL_TRIANGLE_FAN, start_vertex, circle_vertex_count); start_vertex += circle_vertex_count;
	glDrawArrays(GL_TRIANGLE_FAN, start_vertex, circle_vertex_count); start_vertex += circle_vertex_count;
	glDrawArrays(GL_TRIANGLE_STRIP, start_vertex, cylinder_vertex_count); start_vertex += cylinder_vertex_count;
	glDrawArrays(GL_TRIANGLE_STRIP, start_vertex, cylinder_vertex_count);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
