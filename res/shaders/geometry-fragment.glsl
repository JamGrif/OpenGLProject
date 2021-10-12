#version 430 
out vec4 color;

in vec2 varyingTexCoordG;

layout (binding = 0) uniform sampler2D texture_color;

void main()
{
    color = texture(texture_color, varyingTexCoordG);
}