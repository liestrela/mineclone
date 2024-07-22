#ifndef VBO_H
#define VBO_H

#include <stdlib.h>

#include "gfx.h"
#include "vao.h"

typedef struct {
	unsigned handle;
} VBO;

VBO create_vbo(void);
void bind_vbo(VBO vbo);
void buffer_vbo(VBO vbo, VAO vao, GLsizeiptr size, const void *data, GLenum usage);

#endif
