Renderer Showcase Video - https://www.youtube.com/watch?v=QOnscYxgrNQ

Built exe is available at (ProjectDirectory)->bin->x64->Release/Debug->OpenGL.exe
Program can only be compiled in 64bit mode in Visual Studio
The Release version is recommended as while both versions are visually the same, the Release version has superior performance and loading times

Features:
ADS Blinn-Phong light reflection model, Diffuse Map, Specular Map, Emission Map, Height Map, Normal Map, skybox cubemap, 
reflection environment map, refraction environment map, Mountains via height map with Tessellation shaders, 
Model inflation via Geometry shader, post-processing screen filters, coloured lighting, Directional/Point/Spot light, load external 3D models,
load external textures, load external shaders, Parallax mapping, sprite blending, mouse + keyboard input, camera movement on all 3 axis

Controls:
W/A/S/D to move around the camera
Moving the mouse moves the front facing vector of the camera
Q lowers surrounding mountain elevation
E raises surrounding mountain elevation
1/2/3/4/5 changes the screen filter applied to the drawn frame
	1 Normal 
	2 Inverse
	3 Greyscale
	4 Edge detection
	5 ???
6 toggles the scenes directional light on and off
7 toggles the scenes spotlight on and off - this light is off by default and is attached to the camera, acting as a sort of torch
8/9/0/- toggles the scenes point lights on and off
	8 is the point light in the starting house
	9 is the point light moving along the X and Z axis showing the different materials on the floor
	0 is the point light moving along the Z axis showing the difference between using a normal map and no normal map
	- is the colour changing light showcasing coloured lighting

