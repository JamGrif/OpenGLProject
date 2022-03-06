#include "models/ModelTerrain.h"


ModelTerrain::ModelTerrain()
	:m_terrainTexture(nullptr), m_terrainHeight(nullptr), m_elevation(2.5), m_minElevation(-8), m_maxElevation(-3)
{
	setShaderTwo("res/shaders/terrain-vertex.glsl", "res/shaders/terrain-tessCont.glsl", "res/shaders/terrain-tessEval.glsl", "res/shaders/terrain-fragment.glsl");

	//m_terrainTexture = TextureManager::retrieveTexture("res/textures/terrain_diff.png");
	//m_terrainHeight = TextureManager::retrieveTexture("res/textures/terrain_height.png");
}

ModelTerrain::~ModelTerrain()
{
	if (m_terrainTexture)
	{
		m_terrainTexture = nullptr;
	}

	if (m_terrainHeight)
	{
		m_terrainHeight = nullptr;
	}
}

/// <summary>
/// Overridden method from Model base class - Unused in this class
/// </summary>
void ModelTerrain::drawPassOne()
{
}

/// <summary>
/// Overridden method from Model base class - Used to draw surrounding terrain with the tessellation shader
/// </summary>
void ModelTerrain::drawPassTwo()
{
	// If no shader attached
	if (!m_modelShaderPassTwo)
	{
		return;
	}

	// Bind shaders
	m_modelShaderPassTwo->Bind(); 

	/*
		Set Vertex / Tessellation / Fragment values
	*/

	glm::mat4 mMVP = m_localProjectionMatrix * m_vMat * m_mMat;

	m_modelShaderPassTwo->setUniformMatrix4fv("mvp_matrix", mMVP);
	m_modelShaderPassTwo->setUniform1f("elevation", m_elevation);
	m_modelShaderPassTwo->setUniform1i("texture_color", 0);
	m_modelShaderPassTwo->setUniform1i("texture_height", 1);

	m_terrainTexture->Bind(0);
	m_terrainHeight->Bind(1);

	/*
		Draw
	*/
	
	glPatchParameteri(GL_PATCH_VERTICES, 16);
	glDrawArrays(GL_PATCHES, 0, 16);

	/*
		Post-draw cleanup
	*/

	m_modelShaderPassTwo->Unbind();
	m_terrainTexture->Unbind();
	m_terrainHeight->Unbind();
}

void ModelTerrain::setTerrainTexture(const std::string& texturePath)
{
	m_terrainTexture = TextureManager::retrieveTexture(texturePath);
}

void ModelTerrain::setTerrainHeightTexture(const std::string& texturePath)
{
	m_terrainHeight = TextureManager::retrieveTexture(texturePath);
}

/// <summary>
/// Overloaded version of the base setShaderTwo which is able to take in tessellation shader paths
/// </summary>
/// <param name="vertexPath"></param>
/// <param name="tessellationControlPath"></param>
/// <param name="tessellationEvaluationPath"></param>
/// <param name="fragmentPath"></param>
void ModelTerrain::setShaderTwo(const char* vertexPath, const char* tessellationControlPath, const char* tessellationEvaluationPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::retrieveShader(vertexPath, tessellationControlPath, tessellationEvaluationPath, fragmentPath);
}

/// <summary>
/// Sets the elevation of the terrain to a specified value
/// </summary>
/// <param name="value">Specified value to set elevation to</param>
void ModelTerrain::setElevation(float value)
{
	m_elevation = value;
}

/// <summary>
/// Used to change the elevation of the generated terrain. Limits the elevation to a range
/// </summary>
/// <param name="change">Specified change in elevation</param>
void ModelTerrain::alterElevation(float change)
{
		m_elevation += change;

		if (m_elevation < m_minElevation)
				m_elevation = m_minElevation;

		if (m_elevation > m_maxElevation)
				m_elevation = m_maxElevation;
}
