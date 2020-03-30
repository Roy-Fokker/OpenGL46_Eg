#pragma once

#include <glad/glad.h>

namespace opengl46_eg
{
	struct gl_vertex_array
	{
		gl_vertex_array(const float *data, const GLsizeiptr size);
		~gl_vertex_array();

		void use();
	private:
		GLuint object{}, buffer{};
	};
}