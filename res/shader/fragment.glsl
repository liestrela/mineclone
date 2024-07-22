#version 330 core
out vec4 frag_color;

in vec2 tex_coord;
in vec3 color;

uniform sampler2D in_texture;

void main()
{
	frag_color = texture(in_texture, tex_coord);
}

