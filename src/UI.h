#pragma once



#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>


// A wrapper around the imGui to display UI elements to the window
class UI
{
public:
	UI();
	~UI();

	void startOfFrame();
	void drawInFrame();

	void toggleUI();

private:
	bool m_uiVisible;
};

