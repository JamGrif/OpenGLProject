#version 430 
layout (location = 0) in vec3 vertPos;
layout (location = 2) in vec2 vertTex;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec2 varyingTexCoord;

void main()
{
    gl_Position = proj_matrix * v_matrix * m_matrix * vec4(vertPos, 1.0f);
    varyingTexCoord = vertTex;
}