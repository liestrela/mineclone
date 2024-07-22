#ifndef VAO_H
#define VAO_H

#include <stdbool.h>
#include <stdlib.h>

#include "gfx.h"

typedef struct {
	unsigned handle;
} VAO;

VAO create_vao(void);
void bind_vao(VAO vao);
void attrib_vao(VAO vao, unsigned index, int size, GLenum type,
				bool normalized, GLsizei stride, const void *pointer);

#endif
