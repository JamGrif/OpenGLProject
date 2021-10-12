#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "EngineStatics.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

// Abstract model class - The different classes that derive from Model all have their own implementation of
// drawing, such as different shader values, attributes and texture map types. Essentially, provides an
// interface for all drawable objects.
class Model
{
public:
	Model(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~Model();

	virtual void	drawPassOne() = 0;
	virtual void	drawPassTwo() = 0;

	void			setMesh(const char* meshFilePath);
	void			setShaderOne(const char* vertexPath, const char* fragmentPath);
	void			setShaderTwo(const char* vertexPath, const char* fragmentPath);
	
	void			SetXPos(float num), SetYPos(float num), SetZPos(float num);
	float			GetXPos(), GetYPos(), GetZPos();

	void			SetXRot(float num), SetYRot(float num), SetZRot(float num);
	float			GetXRot(), GetYRot(), GetZRot();

	void			SetXScale(float num), SetYScale(float num), SetZScale(float num);
	float			GetXScale(), GetYScale(), GetZScale();

	void			IncXPos(float num), IncYPos(float num), IncZPos(float num);
	void			DecXPos(float num), DecYPos(float num), DecZPos(float num);

	void			IncXRot(float num), IncYRot(float num), IncZRot(float num);
	void			DecXRot(float num), DecYRot(float num), DecZRot(float num);

	void			IncXScale(float num), IncYScale(float num), IncZScale(float num);
	void			DecXScale(float num), DecYScale(float num), DecZScale(float num);

	void			setMatrixValues();

protected:

	
	void			setVBOAttrib(bool shaderPos = false, bool shaderNorm = false, bool shaderTex = false, bool shaderTan = false, bool shaderBiTan = false);
	
	//Model properties
	Mesh*			m_modelMesh;
	Shader*			m_modelShaderPassOne;
	Shader*			m_modelShaderPassTwo;

	GLuint			m_VBO;
	GLuint			m_EBO;

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
	
};

