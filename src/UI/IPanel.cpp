#include "pch.h"
#include "UI/IPanel.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

IPanel::IPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag)
	:m_panelName(panelName), m_imGuiWindowFlag(imGuiWindowFlag)
{
}

IPanel::~IPanel()
{
}

void IPanel::StartOfFrame()
{
	ImGui::Begin(m_panelName.c_str(), NULL, m_imGuiWindowFlag);
}

void IPanel::EndOfFrame()
{
	ImGui::End();
}
