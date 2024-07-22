#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 in_tex_coord;

uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 proj;

out vec3 color;
out vec2 tex_coord;

void main()
{
	gl_Position = proj * view * model * vec4(pos, 1.0);
	color = vec3(0.0, 0.0, 0.0);
	tex_coord = in_tex_coord;
}
