#include "pch.h"
#include "UI/IPanel.h"

IPanel::IPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible)
	:m_panelName(panelName), m_imGuiWindowFlag(imGuiWindowFlag), m_bVisible(bVisible)
{
}

IPanel::~IPanel()
{
}
