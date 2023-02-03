#pragma once
#include "UI/IPanel.h"

class ControlPanel :
	public IPanel
{
public:
	ControlPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag);
	~ControlPanel();

	void Render() override; 

	void SceneChange() override;

private:
};

