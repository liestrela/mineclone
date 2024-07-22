#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"
#include "util.h"

#include "stb_image.h"

typedef struct {
	unsigned handle;
	int width, height, channels;
} Texture;

Texture create_texture(const char *filename);
void bind_texture(Texture tex);

static const char *tex_files[] = {
	"res/grass.png",
	"res/cobble.png"
};

#endif
