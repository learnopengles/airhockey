#include "program.h"
#include "platform_gl.h"

TextureProgram get_texture_program(GLuint program)
{
	return (TextureProgram) {
			program,
			glGetAttribLocation(program, "a_Position"),
			glGetAttribLocation(program, "a_TextureCoordinates"),
			glGetUniformLocation(program, "u_MvpMatrix"),
			glGetUniformLocation(program, "u_TextureUnit")};
}

ColorProgram get_color_program(GLuint program)
{
	return (ColorProgram) {
			program,
			glGetAttribLocation(program, "a_Position"),
			glGetUniformLocation(program, "u_MvpMatrix"),
			glGetUniformLocation(program, "u_Color")};
}
