#include "pch.h"
#include "Scene/EntityTypes/GeometryEntity.h"

GeometryEntity::GeometryEntity()
	:m_geometryTexture(nullptr), m_inflation(0), m_increasing(0)
{
	setShaderTwo("res/shaders/geometry-vertex.glsl", "res/shaders/geometry-geometry.glsl", "res/shaders/geometry-fragment.glsl");

	m_geometryTexture = TextureManager::retrieveTextureObject("res/textures/barrel_diff.png");
}

GeometryEntity::~GeometryEntity()
{
	m_geometryTexture = nullptr;
}

void GeometryEntity::initEntity()
{
	m_modelMesh->setVertexAttributes(true, true, true, false);
}

void GeometryEntity::updateEntity()
{
}

/// <summary>
/// Overridden method from Model base class - Unused in this class
/// </summary>
void GeometryEntity::drawPassOne()
{
}

void GeometryEntity::drawPassTwo()
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
		m_inflation += 0.025f * static_cast<float>(ApplicationClock::getDeltaTime());
		if (m_inflation > 0.2f)
		{
			m_inflation = 0.2f;
			m_increasing = false;
		}
	}
	else
	{
		m_inflation -= 0.025f * static_cast<float>(ApplicationClock::getDeltaTime());
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

	m_modelMesh->Bind();

	/*
		Draw
	*/

	m_localRenderer->draw(m_modelMesh->getIndices().size());

	/*
		Post-draw cleanup
	*/
	m_modelMesh->Unbind();

	m_modelShaderPassTwo->Unbind();
	m_geometryTexture->Unbind();
}

/// <summary>
/// Overloaded function from base model class which allows for geometry shader paths to be added
/// </summary>
/// <param name="vertexPath"></param>
/// <param name="geometryPath"></param>
/// <param name="fragmentPath"></param>
void GeometryEntity::setShaderTwo(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::retrieveShader(vertexPath, geometryPath, fragmentPath);
}
