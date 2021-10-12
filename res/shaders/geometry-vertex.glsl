#version 430 
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoords;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec3 varyingNormal;
out vec2 varyingTexCoord;

void main()
{
    //
    varyingNormal = mat3(transpose(inverse(m_matrix)))*vertNormal;
    varyingTexCoord = vertTexCoords;

    gl_Position = v_matrix * m_matrix * vec4(vertPos, 1.0f);
}