#version 430
//Tessellation Control Shader - controls the topology of the mesh produced. Executes once per incoming vertex

layout (vertices = 1) out; //Number of vertices "per patch" being passed from the vertex shader to control shader

void main()
{
    int TL = 32;
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = TL; gl_TessLevelOuter[2] = TL; 
        gl_TessLevelOuter[1] = TL; gl_TessLevelOuter[3] = TL; 
        gl_TessLevelInner[0] = TL; gl_TessLevelInner[1] = TL; 
    }
}