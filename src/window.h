#ifndef WINDOW_H
#define WINDOW_H

#include "gfx.h"
#include "input.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "texture.h"
#include "block.h"
#include "util.h"

typedef struct {
	GLFWwindow *handle;
	int width, height;
	double delta_time, current_time;
	unsigned n_frames;
	bool wireframe;
	Shader shader;
	VAO vao;
	VBO vbo;
	Texture *textures;
	struct Block *blocks;
	unsigned n_blocks; /* for testing */
} Window;

void count_fps(void);
void fb_size_callback(GLFWwindow *window, int width, int height);
void init_window(int width, int height, const char *title);
void window_loop(void);

extern Window window;

#endif
