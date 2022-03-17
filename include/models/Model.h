#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

#include <string>
#include <memory>

#include "Camera.h"
#include "rendering/Mesh.h"
#include "rendering/Shader.h"
#include "rendering/Texture.h"
#include "EngineStatics.h"

// Abstract model class - The different classes that derive from Model all have their own implementation of
// drawing, such as different shader values, attributes and texture map types. Essentially, provides an
// interface for all drawable objects while providing utility functions they all use.
class Model
{
public:
	Model();
	virtual ~Model();

	virtual void	drawPassOne() = 0;
	virtual void	drawPassTwo() = 0;

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

protected:
	
	// Model properties
	std::shared_ptr<Mesh>		m_modelMesh;
	std::shared_ptr<Shader>		m_modelShaderPassOne;
	std::shared_ptr<Shader>		m_modelShaderPassTwo;

	//World Space
	glm::vec3		m_position;
	glm::vec3		m_rotation;
	glm::vec3		m_scale;

	glm::mat4		m_mMat;		//Model
	glm::mat4		m_vMat;		//View
	glm::mat4		m_tMat;		//Translation
	glm::mat4		m_rMat;		//Rotation
	glm::mat4		m_sMat;		//Scale

	//Cached other classes
	LightManager*	m_localLightManager;
	glm::mat4*		m_localProjectionMatrix;
};

