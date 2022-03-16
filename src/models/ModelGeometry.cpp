#include "models/ModelGeometry.h"

#include "GameTimer.h"

ModelGeometry::ModelGeometry()
	:m_geometryTexture(nullptr), m_inflation(0), m_increasing(0)
{
	setShaderTwo("res/shaders/geometry-vertex.glsl", "res/shaders/geometry-geometry.glsl", "res/shaders/geometry-fragment.glsl");

	m_geometryTexture = TextureManager::retrieveTexture("res/textures/barrel_diff.png");
}

ModelGeometry::~ModelGeometry()
{
	m_geometryTexture = nullptr;
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
		return;
	
	// Bind shader
	m_modelShaderPassTwo->Bind();
	m_geometryTexture->Bind();

	// Determine change of mesh inflation
	if (m_increasing)
	{
		m_inflation += 0.025f * static_cast<float>(EngineStatics::getGameTimer()->getDeltaTime());
		if (m_inflation > 0.2f)
		{
			m_inflation = 0.2f;
			m_increasing = false;
		}
	}
	else
	{
		m_inflation -= 0.025f * static_cast<float>(EngineStatics::getGameTimer()->getDeltaTime());
		if (m_inflation < -0.2f)
		{
			m_inflation = -0.2f;
			m_increasing = true;
		}
	}
		
	/*
		Set Vertex values
	*/

	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());
	m_modelShaderPassTwo->setUniform1f("inflation", m_inflation);
	m_modelShaderPassTwo->setUniform1i("texture_color", 0);

	/*
		Bind VBOs and vertex attributes
	*/

	setVBOAttrib(true, true, true, false, false);

	/*
		Draw
	*/

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_modelMesh->getIndices().size()), GL_UNSIGNED_INT, 0);

	/*
		Post-draw cleanup
	*/

	m_modelShaderPassTwo->Unbind();
	m_geometryTexture->Unbind();
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
