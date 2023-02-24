#version 430 
//Geometry shader allows the manipulation of one primitive(triangle) at a time (access to all 3 vertices of a triangle)

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec3 varyingNormal[];
in vec2 varyingTexCoord[];
out vec3 varyingNormalG;
out vec2 varyingTexCoordG;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float inflation;

void main()
{
    
    for (int i = 0; i < 3; i++)
    {
        //Moves the pixel depending on inflation amount
        gl_Position = proj_matrix * (gl_in[i].gl_Position + normalize(vec4(varyingNormal[i], 1.0)) * inflation);
        varyingNormalG = varyingNormal[i];
        varyingTexCoordG = varyingTexCoord[i];
        EmitVertex(); //Pass vertex to the vertex stream
    }
    EndPrimitive(); //Complete the primitive after 3 vertices have been processed
}