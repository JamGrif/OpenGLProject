#pragma once
#include "UI/IPanel.h"

class ControlPanel :
	public IPanel
{
public:
	ControlPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible);
	~ControlPanel();

	virtual void Update() override;
	virtual void Render() override; 
	virtual void SceneChange() override;

private:
};

