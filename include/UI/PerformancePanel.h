#pragma once
#include "UI/IPanel.h"

class PerformancePanel :
	public IPanel
{
public:
	PerformancePanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag);
	~PerformancePanel();

	void Render() override;

	void SceneChange() override;

private:
};

