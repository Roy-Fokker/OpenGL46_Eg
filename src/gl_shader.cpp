#include "gl_shader.h"

using namespace opengl46_eg;

gl_shader::gl_shader(const char *vs_source, const char *fs_source)
{
	// Vertex Shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, nullptr);
	glCompileShader(vs);

	// Fragment Shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, nullptr);
	glCompileShader(fs);

	// Create and Use Shader Program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);

	glBindFragDataLocation(shader_program, 0, "outColor");

	glLinkProgram(shader_program);
	
	// Vertex Data Layout
	position_attribute = glGetAttribLocation(shader_program, "position");
	glEnableVertexAttribArray(position_attribute);
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

gl_shader::~gl_shader()
{
	// Delete shader stuff
	glDeleteProgram(shader_program);
	glDeleteShader(fs);
	glDeleteShader(vs);
}

void gl_shader::use()
{
	glUseProgram(shader_program);
}