#include "vao.h"

VAO
create_vao(void)
{
	VAO vao;

	glGenVertexArrays(1, &vao.handle);

	return vao;
}

void
bind_vao(VAO vao)
{
	glBindVertexArray(vao.handle);
}

void
attrib_vao(VAO vao, unsigned index, int size, GLenum type,
		   bool normalized, GLsizei stride, const void *pointer)
{
	bind_vao(vao);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
}
