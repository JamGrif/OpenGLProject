#pragma once
#include "UI/IPanel.h"

class SceneModelsPanel :
    public IPanel
{
public:
	SceneModelsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag);
	~SceneModelsPanel();

	virtual void Render() override;

	virtual void SceneChange() override;

	unsigned int GetSelectedModelIndex() { return m_selectedModelIndex; }

private:

	unsigned int m_selectedModelIndex;
};

