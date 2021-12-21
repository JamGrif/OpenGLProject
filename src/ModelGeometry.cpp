#include "ModelGeometry.h"

ModelGeometry::ModelGeometry()
	:m_inflation(0)
{
	setShaderTwo("res/shaders/geometry-vertex.glsl", "res/shaders/geometry-geometry.glsl", "res/shaders/geometry-fragment.glsl");

	m_GeometryTexture = TextureManager::retrieveTexture("res/textures/barrel_diff.png");
}

ModelGeometry::~ModelGeometry()
{
	m_GeometryTexture = nullptr;
}

/// <summary>
/// Overridden method from Model base class - Unused in this class
/// </summary>
void ModelGeometry::drawPassOne()
{
}


void ModelGeometry::drawPassTwo()
{
	//If no shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	//Bind shader
	m_modelShaderPassTwo->Bind();
	m_GeometryTexture->Bind();

	if (m_increasing)
	{
		m_inflation += 0.025f * EngineStatics::getDeltaTime();
		if (m_inflation > 0.2f)
		{
			m_inflation = 0.2f;
			m_increasing = false;
		}
	}
	else
	{
		m_inflation -= 0.025f * EngineStatics::getDeltaTime();
		if (m_inflation < -0.2f)
		{
			m_inflation = -0.2f;
			m_increasing = true;
		}
	}
		
	//Set Vertex values
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());
	m_modelShaderPassTwo->setUniform1f("inflation", m_inflation);
	m_modelShaderPassTwo->setUniform1i("texture_color", 0);

	setVBOAttrib(true, true, true, false, false);

	//Draw
	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	m_modelShaderPassTwo->Unbind();
	m_GeometryTexture->Unbind();
}

/// <summary>
/// Overloaded function from base model class which allows for geometry shader paths to be added
/// </summary>
/// <param name="vertexPath"></param>
/// <param name="geometryPath"></param>
/// <param name="fragmentPath"></param>
void ModelGeometry::setShaderTwo(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::retrieveShader(vertexPath, geometryPath, fragmentPath);
}
