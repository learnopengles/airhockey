#include "platform_gl.h"
#include "program.h"
#include "linmath.h"

typedef struct {
	GLuint texture;
	GLuint buffer;
} Table;

typedef struct {
	vec4 color;
	GLuint buffer;
	int circle_vertex_start;
	int circle_vertex_count;
	int cylinder_vertex_start;
	int cylinder_vertex_count;
} Puck;

typedef struct {
	vec4 color;
	GLuint buffer;
	int circle_vertex_start[2];
	int circle_vertex_count;
	int cylinder_vertex_start[2];
	int cylinder_vertex_count;
} Mallet;

Table create_table(GLuint texture);
void draw_table(const Table* table, const TextureProgram* texture_program, mat4x4 m);

Puck create_puck(float radius, float height, float num_points, vec4 color);
void draw_puck(const Puck* puck, const ColorProgram* color_program, mat4x4 m);

Mallet create_mallet(float radius, float height, float num_points, vec4 color);
void draw_mallet(const Mallet* mallet, const ColorProgram* color_program, mat4x4 m);



