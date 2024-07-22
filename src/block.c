#include "block.h"

void
setup_block(void)
{
	bind_vao(window.vao);
	bind_vbo(window.vbo);

	buffer_vbo(window.vbo, window.vao, sizeof(block_vertex), block_vertex,
			   GL_STATIC_DRAW);

	attrib_vao(window.vao, 0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float),
			   (void *)(0));
	attrib_vao(window.vao, 1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float),
			   (void *)(3*sizeof(float)));
}

struct Block
create_block(uint8_t id, vec3 pos)
{
	struct Block block;
	mat4 model;

	block.id = id;
	glm_vec3_copy(pos, block.pos);

	glm_mat4_identity(model);
	glm_translate(model, pos);

	return block;
}
