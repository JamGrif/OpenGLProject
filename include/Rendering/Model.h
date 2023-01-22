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
	Model(const ModelLoaderParams& pParams);
    ~Model();

	void			UpdateModel();
	void			DrawModel();

	void			SetModelPointers(SceneCamera* pSceneCamera);

	inline void		SetMatrixValues();

private:

	// For MeshManager - The mesh this model uses
	std::string		m_meshID;

	// For MaterialManager - The material this model uses
	std::string		m_materialID;

	// World Space
	Vector3D		m_position;
	Vector3D		m_rotation;
	Vector3D		m_scale;

	glm::mat4		m_mMat;		// Model
	glm::mat4		m_vMat;		// View
	glm::mat4		m_tMat;		// Translation
	glm::mat4		m_rMat;		// Rotation
	glm::mat4		m_sMat;		// Scale

	SceneCamera* m_pSceneCamera;

	const glm::mat4& m_programProjectionMatrix;
};

