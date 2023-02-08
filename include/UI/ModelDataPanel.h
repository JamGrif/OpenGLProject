#pragma once
#include "UI/IPanel.h"

class SceneModelsPanel;

struct SelectedModelCache;

/// <summary>
/// Displays data related to a selected model from the SceneModels panel
/// </summary>
class ModelDataPanel :
    public IPanel
{
public:
	ModelDataPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer, std::weak_ptr<SceneModelsPanel> pSceneModelPanel);
	~ModelDataPanel();

	virtual void						Update() override;
	virtual void						Render() override;
	virtual void						SceneChange() override;

private:

	void								SetupModelCache();
	void								ClearModelCache();

	void								DeleteModel_BUTTON();
	void								GotoModel_BUTTON(const Vector3D& newPos);
	void								ClosePanel_BUTTON();

	// Pointer to the SceneModels panel
	// Lets this panel check if a new model has been selected
	std::weak_ptr<SceneModelsPanel>		m_pSceneModelPanel;

	std::unique_ptr<SelectedModelCache> m_pModelCache;

	// Index of the currently selected model in the SceneModels in Scene
	int									m_selectedModelIndex;
};
