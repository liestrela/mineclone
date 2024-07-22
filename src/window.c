#include "window.h"

Window window;

void
count_fps(void)
{
	window.current_time = glfwGetTime();
	window.n_frames++;

	if (window.current_time-window.delta_time>=1) {
		printf("%.2lf ms/f (%u f/s)\n", 1000.0/window.n_frames, 
			   window.n_frames);
		window.n_frames = 0;
		window.delta_time++;
	}
}

void
fb_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void
init_window(int width, int height, const char *title)
{
	unsigned n_textures;	

	if (!glfwInit()) die("error: couldn't init glfw\n");
	
	window.width  = width;
	window.height = height;
	window.wireframe = false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window.handle = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window.handle) die("error: couldn't create window\n");

	glfwMakeContextCurrent(window.handle);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		die("error: couldn't init glad\n");
	
	glfwSetFramebufferSizeCallback(window.handle, fb_size_callback);

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	window.vao = create_vao();	
	window.vbo = create_vbo();

	setup_block();

	n_textures = sizeof(tex_files)/sizeof(tex_files[0]);

	window.textures = (Texture *)malloc(n_textures*sizeof(Texture));
	
	for (unsigned i=0; i<n_textures; i++)
		window.textures[i] = create_texture(tex_files[i]);


	/* temp manual block creation for testing */
	vec3 temp_pos[] = {
		{ 0, 0, -5}, { 1, 0, -5},
		{ 0, 0, -6}, { 1, 0, -6},
		{ 0, 1, -5}, { 1, 1, -5},
		{ 0, 1, -6}, { 1, 1, -6},
		{-1, 0, -5}, {-1, 0, -6},
		{-1, 1, -5}, {-1, 1, -6}
	};

	window.n_blocks = sizeof(temp_pos)/sizeof(temp_pos[0]);

	window.blocks = (struct Block *)malloc(window.n_blocks*sizeof(struct Block));

	for (unsigned i=0; i<window.n_blocks; i++)
		window.blocks[i] = create_block(0, temp_pos[i]);

	window.shader = create_shader("res/shader/vertex.glsl",
								  "res/shader/fragment.glsl");
	glUseProgram(window.shader.id);

	window.delta_time = glfwGetTime();
}

void
window_loop(void)
{
	int model_loc, view_loc, proj_loc;
	mat4 model, view, proj;

	glm_mat4_identity(model);
	
	glm_perspective(45, 1, 0.1, 100, proj);

	while (!glfwWindowShouldClose(window.handle)) {
		count_fps();
		handle_inputs();

		glClearColor(0.5, 0.8, 1.0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(window.shader.id);
		
		model_loc = glGetUniformLocation(window.shader.id, "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, (const GLfloat *)model);

		glm_lookat((vec3){0, 0, 0}, (vec3){0, 0, -1}, (vec3){0, 1, 0}, view);
		view_loc = glGetUniformLocation(window.shader.id, "view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, (const GLfloat *)view);

		proj_loc = glGetUniformLocation(window.shader.id, "proj");
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, (const GLfloat *)proj);

		bind_texture(window.textures[0]);
		bind_vao(window.vao);

/*		glm_mat4_identity(model);
		glm_translate(model, pos);
		glDrawArrays(GL_TRIANGLES, 0, 36);
*/

		for (unsigned i=0; i<window.n_blocks; i++) {
			struct Block block;
			mat4 block_model;

			block = window.blocks[i];
			bind_texture(window.textures[block.id]);

			glm_mat4_identity(block_model);
			glm_translate(block_model, block.pos);
			glUniformMatrix4fv(model_loc, 1, GL_FALSE, 
							   (const GLfloat *)block_model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	glfwTerminate();
}
