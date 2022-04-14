#pragma once

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

#include "Scene/SceneCamera.h"
#include "Core/EngineStatics.h"

#include "Rendering/OpenGLMesh.h"
#include "Rendering/OpenGLShader.h"
#include "Rendering/OpenGLTexture.h"
#include "Rendering/OpenGLCubemap.h"

#include "Rendering/Renderer.h"

struct templateBaseEntity
{
	std::string objectType = ""; // ModelLighting, ModelBasic, ModelTerrain etc...

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

// Abstract model class - The different classes that derive from Model all have their own implementation of
// drawing, such as different shader values, attributes and texture map types. Essentially, provides an
// interface for all drawable objects while providing utility functions they all use.
class BaseEntity
{
public:
	BaseEntity();
	BaseEntity(templateBaseEntity object);
	virtual ~BaseEntity();

	virtual void	initEntity()	= 0;
	virtual void	updateEntity()	= 0;
	virtual void	drawPassOne()	= 0;
	virtual void	drawPassTwo()	= 0;

	void			setMesh(const std::string& meshFilePath);
	void			setShaderOne(const char* vertexPath, const char* fragmentPath);
	void			setShaderTwo(const char* vertexPath, const char* fragmentPath);
	
	void			SetXPos(float num),		SetYPos(float num),		SetZPos(float num);
	float			GetXPos() const,		GetYPos() const,		GetZPos() const;

	void			SetXRot(float num),		SetYRot(float num),		SetZRot(float num);
	float			GetXRot() const,		GetYRot() const,		GetZRot() const;

	void			SetXScale(float num),	SetYScale(float num),	SetZScale(float num);
	float			GetXScale() const,		GetYScale() const,		GetZScale() const;

	void			IncXPos(float num),		IncYPos(float num),		IncZPos(float num);
	void			DecXPos(float num),		DecYPos(float num),		DecZPos(float num);

	void			IncXRot(float num),		IncYRot(float num),		IncZRot(float num);
	void			DecXRot(float num),		DecYRot(float num),		DecZRot(float num);

	void			IncXScale(float num),	IncYScale(float num),	IncZScale(float num);
	void			DecXScale(float num),	DecYScale(float num),	DecZScale(float num);

	void			setMatrixValues();

	void			setEntityType(const std::string& type);
	std::string		getEntityType();

protected:
	
	// Model properties
	std::shared_ptr<OpenGLMesh>			m_modelMesh;
	std::shared_ptr<OpenGLShader>		m_modelShaderPassOne;
	std::shared_ptr<OpenGLShader>		m_modelShaderPassTwo;

	//World Space
	glm::vec3		m_position;
	glm::vec3		m_rotation;
	glm::vec3		m_scale;

	glm::mat4		m_mMat;		// Model
	glm::mat4		m_vMat;		// View
	glm::mat4		m_tMat;		// Translation
	glm::mat4		m_rMat;		// Rotation
	glm::mat4		m_sMat;		// Scale

	std::string		m_entityType;

	//Cached other classes
	std::shared_ptr<SceneLightManager>	m_localLightManager;
	std::shared_ptr<Renderer>			m_localRenderer;
	glm::mat4*							m_localProjectionMatrix;
};

