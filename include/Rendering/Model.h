#pragma once

class SceneCamera;
class SceneLightManager;

#include <glm\mat4x4.hpp>



struct ModelLoaderParams
{
	float posX, posY, posZ;			// Starting position of model
	float rotX, rotY, rotZ;			// Starting rotation of model
	float scaleX, scaleY, scaleZ;	// Starting scale of model

	std::string materialID;			// ID of material that the model will use
	std::string meshID;				// ID of mesh that the model will use
};


// Class that allows an object to be lit by all the scene lights and gives the option for multiple texture maps to be applied
class Model
{
public:
	Model(ModelLoaderParams pParams);
    ~Model();

	void			updateModel();
	void			drawModel();

	void			setModelPointers(SceneCamera* pSceneCamera, SceneLightManager* pSceneLightManager);

	const Vector3D& getPosition() const { return m_position; }

	void			setMatrixValues();

private:

	std::string		m_materialID;
	std::string		m_meshID;

	// World Space
	Vector3D		m_position;
	Vector3D		m_rotation;
	Vector3D		m_scale;

	glm::mat4		m_mMat;		// Model
	glm::mat4		m_vMat;		// View
	glm::mat4		m_tMat;		// Translation
	glm::mat4		m_rMat;		// Rotation
	glm::mat4		m_sMat;		// Scale

	SceneLightManager* m_pSceneLightManager;
	SceneCamera* m_pSceneCamera;

	const glm::mat4& m_programProjectionMatrix;
};

