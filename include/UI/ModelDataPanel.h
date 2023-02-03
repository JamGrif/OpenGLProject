#pragma once
#include "UI/IPanel.h"

class SceneModelsPanel;

class ModelDataPanel :
    public IPanel
{
public:
	ModelDataPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, SceneModelsPanel* pSceneModelPanel);
	~ModelDataPanel();

	virtual void Render() override;

	virtual void SceneChange() override;


private:

	void ClearSelectedEntity();

	SceneModelsPanel* m_pSceneModelPanel;


	bool m_bVisible;
	unsigned int m_selectedModelIndex;

};

