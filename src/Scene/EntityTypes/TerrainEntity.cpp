 #include "pch.h"
#include "Scene/EntityTypes/TerrainEntity.h"

TerrainEntity::TerrainEntity(templateTerrainEntity object)
	:BaseEntity(object), m_terrainTexture(nullptr), m_terrainHeight(nullptr), m_elevation(object.elevation), m_minElevation(-8), m_maxElevation(-3)
{
	setTerrainTexture(object.terrainTextureMap);
	setTerrainHeightTexture(object.terrainHeightMap);

	setShaderTwo("res/shaders/terrain-vertex.glsl", "res/shaders/terrain-tessCont.glsl", "res/shaders/terrain-tessEval.glsl", "res/shaders/terrain-fragment.glsl");
}

TerrainEntity::~TerrainEntity()
{
	m_terrainTexture = nullptr;
	m_terrainHeight = nullptr;
}

void TerrainEntity::initEntity()
{
}

void TerrainEntity::updateEntity()
{
}

/// <summary>
/// Overridden method from BaseEntity class - Unused in this class
/// </summary>
void TerrainEntity::drawPassOne()
{
}

/// <summary>
/// Overridden method from BaseEntity class - Used to draw surrounding terrain with the tessellation shader
/// </summary>
void TerrainEntity::drawPassTwo()
{
	// If no shader attached
	if (!m_modelShaderPassTwo)
		return;
	
	// Bind shaders
	m_modelShaderPassTwo->Bind(); 

	/*
		Set Vertex / Tessellation / Fragment values
	*/

	glm::mat4 mMVP = *m_localProjectionMatrix * m_vMat * m_mMat;

	m_modelShaderPassTwo->setUniformMatrix4fv("mvp_matrix", mMVP);
	m_modelShaderPassTwo->setUniform1f("elevation", m_elevation);
	m_modelShaderPassTwo->setUniform1i("texture_color", 0);
	m_modelShaderPassTwo->setUniform1i("texture_height", 1);

	m_terrainTexture->Bind();
	m_terrainHeight->Bind();

	/*
		Draw
	*/

	m_localRenderer->drawTerrain(16);

	/*
		Post-draw cleanup
	*/

	//m_modelMesh->Unbind();
	m_modelShaderPassTwo->Unbind();
	m_terrainTexture->Unbind();
	m_terrainHeight->Unbind();
}

void TerrainEntity::setTerrainTexture(const std::string& texturePath)
{
	m_terrainTexture = TextureManager::retrieveTextureObject(texturePath);
	m_terrainTexture->setTextureSlot(0);
}

void TerrainEntity::setTerrainHeightTexture(const std::string& texturePath)
{
	m_terrainHeight = TextureManager::retrieveTextureObject(texturePath);
	m_terrainHeight->setTextureSlot(1);
}

/// <summary>
/// Overloaded version of the base setShaderTwo which is able to take in tessellation shader paths
/// </summary>
/// <param name="vertexPath"></param>
/// <param name="tessellationControlPath"></param>
/// <param name="tessellationEvaluationPath"></param>
/// <param name="fragmentPath"></param>
void TerrainEntity::setShaderTwo(const char* vertexPath, const char* tessellationControlPath, const char* tessellationEvaluationPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::retrieveShader(vertexPath, tessellationControlPath, tessellationEvaluationPath, fragmentPath);
}

/// <summary>
/// Sets the elevation of the terrain to a specified value
/// </summary>
/// <param name="value">Specified value to set elevation to</param>
void TerrainEntity::setElevation(float value)
{
	m_elevation = value;
}

/// <summary>
/// Used to change the elevation of the generated terrain. Limits the elevation to a range
/// </summary>
/// <param name="change">Specified change in elevation</param>
void TerrainEntity::alterElevation(float change)
{
		m_elevation += change;

		if (m_elevation < m_minElevation)
				m_elevation = m_minElevation;

		if (m_elevation > m_maxElevation)
				m_elevation = m_maxElevation;
}
