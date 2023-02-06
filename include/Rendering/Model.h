#pragma once

#include <glm\mat4x4.hpp>

struct ModelLoaderParams
{
	std::string modelID;

	Vector3D position;
	Vector3D rotation;
	Vector3D scale;

	std::string materialID;			// ID of material that the model will use
	ResourceID meshID;				// ID of mesh that the model will use
};

// Class that allows an object to be lit by all the scene lights and gives the option for multiple texture maps to be applied
class Model
{
public:
	Model(const ModelLoaderParams& pParams);
    ~Model();

	void			UpdateModel();
	void			DrawModel();

	void			SetModelPointers(std::weak_ptr<SceneCamera> pSceneCamera);

	const std::string& getModelID() const { return m_modelID; }

	const Vector3D& GetPosition() const { return m_position; }
	const Vector3D& GetRotation() const { return m_rotation; }
	const Vector3D& GetScale() const { return m_scale; }

	const ResourceID& GetMeshID() const { return m_meshID; }
	const std::string& GetMaterialID() const { return m_materialID; }

private:

	inline void		SetMatrixValues();

private:

	// Unique ID of model - generated when model is created
	std::string m_modelID;

	// For MeshManager - The mesh this model uses
	ResourceID		m_meshID;

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

	std::weak_ptr<SceneCamera> m_pSceneCamera;

	const glm::mat4& m_programProjectionMatrix;
};

