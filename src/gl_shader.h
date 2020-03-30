#pragma once

#include <glad/glad.h>

namespace  opengl46_eg
{
	struct gl_shader
	{
		gl_shader(const char *vs_source, const char *fs_source);
		~gl_shader();

		void use();

	private:
		GLuint vs{}, fs{}, shader_program{};
		GLint position_attribute;
	};
}