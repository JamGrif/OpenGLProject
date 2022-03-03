#include "UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include "EngineStatics.h"
#include "Input.h"

UI::UI(bool uiVisible)
	:m_uiVisible(uiVisible)
{
	std::cout << "UI made" << std::endl;

	if (m_uiVisible)
	{
		Input::enableMouse();
	}
	else
	{
		Input::disableMouse();
	}

	IMGUI_CHECKVERSION(); // Check the version
	
	ImGui::CreateContext();	// Creating the imgui context
	ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	
	ImGui_ImplGlfw_InitForOpenGL(EngineStatics::getAppWindow()->getWindow(), true); // Connect imgui to glfw window
	ImGui_ImplOpenGL3_Init("#version 430");
	
	ImGui::StyleColorsDark(); 
}

UI::~UI()
{
	std::cout << "UI destroyed" << std::endl;
}

void UI::startOfFrame()
{
	if (m_uiVisible)
	{
		// imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	
}

void UI::drawInFrame()
{
	if (!m_uiVisible)
		return;

	ImGui::Begin("Hello from begin");
	ImGui::Text("Hello from text");
	ImGui::End();

	//ImGui::Begin("Hello from begin2");
	//ImGui::Text("Hello from text2");
	//ImGui::End();
	//
	//ImGui::Begin("Hello from begin3");
	//ImGui::Text("Hello from text3");
	//ImGui::End();
	//
	//ImGui::Begin("Hello from begin4");
	//ImGui::Text("Hello from text4");
	//ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	

}

void UI::toggleUI()
{
	m_uiVisible = m_uiVisible == true ? false : true;

	if (m_uiVisible)
	{
		Input::enableMouse();
	}
	else
	{
		Input::disableMouse();
	}
}

bool UI::getUiVisible()
{
	return m_uiVisible;
}
