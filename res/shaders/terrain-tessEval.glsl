#version 430 
//Tessellation evaluation shader - executed once per vertex produced

layout (quads, equal_spacing, ccw) in; //Unrelated to "out" in control shader, Instruct tessellator to generate vertices arranged in a quad, ccw specifies widning order of vertices and equal spacing of sub divisions

uniform mat4 mvp_matrix;
uniform float elevation;

layout (binding = 1) uniform sampler2D texture_height;
out vec2 tes_out;

void main()
{
	//Map the grid vertices from 0 - 1 range to -0.5 - 0.5 range
	vec4 tessellatedPoint = vec4(gl_TessCoord.x - 0.5, 0.0, gl_TessCoord.y - 0.5, 1.0);

	//Map the grid vertices as texture coordinates (vertex coords 0,0 top left, tex coords 0,0 bottom left
	vec2 tc = vec2(gl_TessCoord.x, 1.0 - gl_TessCoord.y);

	//Set height of vertex by using height map values (greyscale image so only .r value is needed)
	tessellatedPoint.y += (texture(texture_height, tc).r) * elevation; //2.5

	//Convert to eye space
	gl_Position = mvp_matrix * tessellatedPoint;

	tes_out = tc;

}