![](https://github.com/JamGrif/OpenGLProject/blob/main/res/media/banner1.jpg?raw=true)

![](https://github.com/JamGrif/OpenGLProject/blob/main/res/media/banner4.jpg?raw=true)

![](https://github.com/JamGrif/OpenGLProject/blob/main/res/media/banner6.jpg?raw=true)

# OpenGL Project

This project is a continuation of my Final Year Project for University where I created a 3D renderer using OpenGL which included a large variety of graphical features.

I have been further developing this project in my own time for fun by adding new features, fixing bugs and issues, and further abstracting away parts of the program.

This project is mainly a fun way for me to experiment and test my OpenGL and C++ ability.

**OLD** Renderer Showcase Video - https://www.youtube.com/watch?v=QOnscYxgrNQ

# Features

**New features over the original FMP version:**  
- Multithreading for asset loading (textures, meshes & shader sources)  
- Scene textfile loading during runtime  
- ImGui interface with a simple scene editor (Change scene, change screen filter, adjust lights status, view and delete entity values)  
- Improved logging with better timer and error handling  

**Original features:**  
- ADS Blinn-Phong Light Reflection Model  
- Diffuse, Specular, Emission, Height & Normal texture maps
- Coloured lighting from Directional/Point/Spot lights  
- Skybox Cubemap  
- Reflection Environment Map & Refraction Environment Map  
- Mountains via Height Map with Tessellation shaders  
- Model inflation via Geometry shader  
- Post-processing screen filters  
- Load external 3D models, textures & shaders  
- Parallax mapping   
- Sprite blending  
- Mouse + keyboard input with camera movement on all 3 axis  

# Controls

**W/A/S/D** to move around the camera  
Moving the mouse moves the front facing vector of the camera  
**1/2/3/4/5** to change the screen filter applied to the drawn frame  
	1 Normal  
	2 Inverse  
	3 Greyscale  
	4 Edge detection  
	5 ???  
**Q** to toggle the UI

# Building

Built exe is available at (ProjectDirectory)->bin->x64->Release/Debug->OpenGL.exe  
Program can only be compiled in 64bit mode in Visual Studio.  
The Release version is recommended as while both versions are visually the same, the Release version has superior performance and loading times.  
The project should be able to be built straight out of the box as the repository contains the required dependencies.  

**This OpenGL Project is dependant on:**

[assimp](https://github.com/assimp/assimp)

[GLEW](https://github.com/nigels-com/glew)

[GLFW](https://github.com/glfw/glfw)

[glm](https://github.com/g-truc/glm)

[imgui](https://github.com/ocornut/imgui)

[spdlog](https://github.com/gabime/spdlog)

[stb_image](https://github.com/nothings/stb)

![](https://github.com/JamGrif/OpenGLProject/blob/main/res/media/banner5.jpg?raw=true)

![](https://github.com/JamGrif/OpenGLProject/blob/main/res/media/banner2.jpg?raw=true)

![](https://github.com/JamGrif/OpenGLProject/blob/main/res/media/banner3.jpg?raw=true)
