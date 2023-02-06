#pragma once
#include "UI/IPanel.h"

class SceneModelsPanel :
    public IPanel
{
public:
	SceneModelsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible);
	~SceneModelsPanel();

	virtual void	Update() override;
	virtual void	Render() override;
	virtual void	SceneChange() override;

	int				GetSelectedModelIndex() { return m_selectedModelIndex; }
	bool			GetUpdatedSelectedModel() { return m_bNewSelectedModel; }

	void			ClearSelectedModelIndex() { m_selectedModelIndex = -1; }

private:

	// Index of the currently selected model in the SceneModels in Scene
	int				m_selectedModelIndex;

	// Whether a Model button has been pressed and the ModelData panel needs to be updated
	bool			m_bNewSelectedModel;
};

