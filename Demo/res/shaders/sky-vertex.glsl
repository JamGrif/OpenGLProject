#version 430 
layout (location = 0) in vec3 vertPos;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec3 texCoords;

void main()
{
    texCoords = vertPos;
    vec4 pos = proj_matrix * v_matrix * vec4(vertPos, 1.0);
    gl_Position = pos.xyww;
}