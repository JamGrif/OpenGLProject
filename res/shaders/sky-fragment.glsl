#version 430 
out vec4 color;

in vec3 texCoords;



uniform samplerCube sky;


void main()
{
    color = texture(sky, texCoords);
}