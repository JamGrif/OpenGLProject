#version 430 
layout (location=0) in vec3 vertPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main(void)
{
	gl_Position = lightSpaceMatrix * model * vec4(vertPos, 1.0);
}