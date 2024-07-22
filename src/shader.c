#include "shader.h"

Shader
create_shader(const char *vertex_src, const char *fragment_src)
{
	FILE *file;
	char *source_buf;
	long source_size;
	char log[512];
	unsigned vertex, fragment, program;
	int status;
	Shader shader;

	file = fopen(vertex_src, "r");
	if (!file) die("error: couldn't open vertex shader\n");

	source_size = file_size(file);
	source_buf = (char *)malloc(source_size);

	fread(source_buf, source_size, 1, file);
	fclose(file);

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, (const char **)&source_buf, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &status);

	if (!status) {
		glGetShaderInfoLog(vertex, 512, NULL, log);
		die("error: vertex shader compilation error (%s)\n", log);
	}

	free(source_buf);

	file = fopen(fragment_src, "r");
	if (!file) die("error: couldn't open fragment shader\n");

	source_size = file_size(file);
	source_buf = (char *)malloc(source_size);

	fread(source_buf, source_size, 1, file);
	fclose(file);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, (const char **)&source_buf, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &status);

	if (!status) {
		glGetShaderInfoLog(fragment, 512, NULL, log);
		die("error: fragment shader compilation error (%s)\n", log);
	}

	free(source_buf);

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	shader.id = program;

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shader;
}
