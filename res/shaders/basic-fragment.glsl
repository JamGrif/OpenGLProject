#version 430 
out vec4 color;

uniform vec3 blockColour;

void main()
{
    color = vec4(blockColour, 1.0f);
}