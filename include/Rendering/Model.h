#pragma once


#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

#include "Scene/SceneCamera.h"
#include "Core/EngineStatics.h"

#include "Rendering/OpenGLRenderer.h"

struct ModelLoaderParams
{
	std::string objectType = ""; // ModelLighting, ModelBasic, ModelTerrain etc...

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::string materialID;

	std::string meshID;
	std::string meshFilepath;
};


// Class that allows an object to be lit by all the scene lights and gives the option for multiple texture maps to be applied
class Model
{
public:
	Model(ModelLoaderParams pParams);
    ~Model();

	void							initEntity();
	void							updateEntity();
	void							drawPassOne();
	void							drawPassTwo();

	void			SetXPos(float num), SetYPos(float num), SetZPos(float num);
	float			GetXPos() const, GetYPos() const, GetZPos() const;

	void			SetXRot(float num), SetYRot(float num), SetZRot(float num);
	float			GetXRot() const, GetYRot() const, GetZRot() const;

	void			SetXScale(float num), SetYScale(float num), SetZScale(float num);
	float			GetXScale() const, GetYScale() const, GetZScale() const;

	void			IncXPos(float num), IncYPos(float num), IncZPos(float num);
	void			DecXPos(float num), DecYPos(float num), DecZPos(float num);

	void			IncXRot(float num), IncYRot(float num), IncZRot(float num);
	void			DecXRot(float num), DecYRot(float num), DecZRot(float num);

	void			IncXScale(float num), IncYScale(float num), IncZScale(float num);
	void			DecXScale(float num), DecYScale(float num), DecZScale(float num);

	void			setMatrixValues();

	void			setEntityType(const std::string& type);
	std::string		getEntityType() const;

	void			setMaterialID(const std::string& materialID) { m_materialID = materialID; }

	// Editor
	bool			getTransformUpdated();
	bool			getTextureUpdated();

	void			setEditorName(const std::string& name);
	std::string		getEditorName() const;


private:

	std::string m_meshID;


	std::string		m_entityType;
	std::string		m_materialID;


	// World Space
	glm::vec3		m_position;
	glm::vec3		m_rotation;
	glm::vec3		m_scale;

	glm::mat4		m_mMat;		// Model
	glm::mat4		m_vMat;		// View
	glm::mat4		m_tMat;		// Translation
	glm::mat4		m_rMat;		// Rotation
	glm::mat4		m_sMat;		// Scale


	// Editor
	bool			m_transformUpdated;
	bool			m_textureUpdated;

	std::string		m_editorName; // Name of the entity within the ImGui scene editor

	// Cached other classes
	std::shared_ptr<SceneLightManager>	m_localLightManager;
	glm::mat4* m_localProjectionMatrix;
};

