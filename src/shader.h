#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"
#include "util.h"

typedef struct {
	unsigned id;
} Shader;

Shader create_shader(const char *vertex_src, const char *fragment_src);

#endif
