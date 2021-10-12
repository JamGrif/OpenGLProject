#version 430 
out vec4 color;

in vec2 varyingTexCoord;

uniform sampler2D sprite;

void main()
{
    vec4 texColor = texture(sprite, varyingTexCoord);
    if (texColor.a < 0.1) //Discard fragment if alpha value is 0
        discard;
    color = texColor;
}