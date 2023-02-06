#pragma once
#include "UI/IPanel.h"

class PerformancePanel :
	public IPanel
{
public:
	PerformancePanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible);
	~PerformancePanel();

	virtual void Update() override;
	virtual void Render() override;
	virtual void SceneChange() override;

private:
};

