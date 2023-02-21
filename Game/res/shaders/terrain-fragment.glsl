#version 430 
out vec4 color;

in vec2 tes_out;

layout (binding = 0) uniform sampler2D texture_color;

void main()
{
	//Apply texture to grid
	color = texture(texture_color, tes_out);
}