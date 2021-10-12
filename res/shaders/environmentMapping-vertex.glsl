#version 430 
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec3 Normal;
out vec3 Position;

void main()
{
    Normal = mat3(transpose(inverse(m_matrix))) * vertNormal;
    Position = vec3(m_matrix * vec4(vertPos, 1.0));
    gl_Position = proj_matrix * v_matrix * m_matrix * vec4(vertPos, 1.0f);
}