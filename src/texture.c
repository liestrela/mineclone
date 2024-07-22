#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

Texture
create_texture(const char *filename)
{
	Texture texture;
	unsigned char *data;

	data = stbi_load(filename, &texture.width, &texture.height,
				 	 &texture.channels, 0);
	
	glGenTextures(1, &texture.handle);
	glBindTexture(GL_TEXTURE_2D, texture.handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height,
					 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else {
		die("error: couldn't load texture (%s)\n", filename);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texture;
}

void
bind_texture(Texture tex)
{
	glBindTexture(GL_TEXTURE_2D, tex.handle);
}
