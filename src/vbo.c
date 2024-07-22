#include "vbo.h"

VBO
create_vbo(void)
{
	VBO vbo;

	glGenBuffers(1, &vbo.handle);

	return vbo;
}

void
bind_vbo(VBO vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo.handle);
}

void
buffer_vbo(VBO vbo, VAO vao, GLsizeiptr size, const void *data, GLenum usage)
{
	bind_vao(vao);
	bind_vbo(vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
