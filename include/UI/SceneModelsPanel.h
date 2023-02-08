#pragma once
#include "UI/IPanel.h"

const int NO_MODEL_SELECTED = -1;

/// <summary>
/// Displays buttons showing all models in the SceneModels vector in Scene
/// Lets user select a button to display its data in the ModelData panel
/// </summary>
class SceneModelsPanel :
    public IPanel
{
public:
	SceneModelsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer);
	~SceneModelsPanel();

	virtual void	Update() override;
	virtual void	Render() override;
	virtual void	SceneChange() override;

	int				GetSelectedModelIndex() { return m_selectedModelIndex; }
	bool			GetUpdatedSelectedModel() { return m_bNewSelectedModel; }

	void			ClearSelectedModelIndex() { m_selectedModelIndex = NO_MODEL_SELECTED; }

private:

	// Index of the currently selected model in the SceneModels in Scene
	int				m_selectedModelIndex;

	// Whether a Model button has been pressed and the ModelData panel needs to be updated
	bool			m_bNewSelectedModel;
};
