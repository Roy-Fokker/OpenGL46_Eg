#include "gl_vertex_array.h"

using namespace opengl46_eg;

gl_vertex_array::gl_vertex_array(const float *data, const GLsizeiptr size)
{
	// Vertex Array Object
	glGenVertexArrays(1, &object);
	glBindVertexArray(object);

	// Vertex Buffer Object
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
				size * sizeof(GLfloat),
				data,
				GL_STATIC_DRAW);
}

gl_vertex_array::~gl_vertex_array()
{
	// Delete vertex stuff
	glDeleteBuffers(1, &buffer);
	glDeleteVertexArrays(1, &object);
}

void gl_vertex_array::use()
{
	glBindVertexArray(object);
}