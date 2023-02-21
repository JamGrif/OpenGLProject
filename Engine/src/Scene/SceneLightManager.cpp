#include "pch.h"
#include "Scene/SceneLightManager.h"

static constexpr int DEFAULT_DIRECTIONAL_LIGHT_AMOUNT	= 1;
static constexpr int DEFAULT_POINT_LIGHT_AMOUNT			= 4;
static constexpr int DEFAULT_SPOT_LIGHT_AMOUNT			= 1;

SceneLightManager::SceneLightManager()
	:m_maxDirectionalLights(DEFAULT_DIRECTIONAL_LIGHT_AMOUNT),
	m_maxPointLights(DEFAULT_POINT_LIGHT_AMOUNT),
	m_maxSpotLights(DEFAULT_SPOT_LIGHT_AMOUNT)
{
}

SceneLightManager::~SceneLightManager()
{
}

/// <summary>
/// Set the direction of a specified directional light
/// </summary>
void SceneLightManager::SetDirectionalLight(Vector3D position, unsigned int index)
{
	if (m_sceneDirectionalLights.at(index))
		m_sceneDirectionalLights.at(index)->m_direction = position;
}

/// <summary>
/// Create a new directional light with specified starting data
/// </summary>
void SceneLightManager::AddDirectionalLight(std::shared_ptr<DirectionalLoaderParams> pParams)
{
	// Check available space
	if (m_sceneDirectionalLights.size() < m_maxDirectionalLights)
		m_sceneDirectionalLights.emplace_back(std::make_shared<DirectionalLight>(pParams));
	else
		PRINT_WARN("LIGHTMANAGER-> Can't create new directional light with direction {0} , {1} , {2}",
			pParams->direction.GetX(), pParams->direction.GetY(), pParams->direction.GetZ());
}

/// <summary>
/// Return directional light at index
/// </summary>
std::weak_ptr<DirectionalLight> SceneLightManager::GetDirectionalLight(unsigned int index) const
{
	if (m_sceneDirectionalLights.size() != 0 && index < m_sceneDirectionalLights.size())
		return m_sceneDirectionalLights.at(index);
	else
		return {};
}

/// <summary>
/// Set the 3D position of a specified point light
/// </summary>
void SceneLightManager::SetPointLight(Vector3D position, unsigned int index)
{
	if (m_scenePointLights.at(index))
		m_scenePointLights.at(index)->m_position = position;
}

/// <summary>
/// Create a new point light with specified starting data
/// </summary>
void SceneLightManager::AddPointLight(std::shared_ptr<PointLoaderParams> pParams)
{
	// Check available space
	if (m_scenePointLights.size() < m_maxPointLights)
		m_scenePointLights.emplace_back(std::make_shared<PointLight>(pParams));
	else
		PRINT_WARN("LIGHTMANAGER-> Can't create new point light with direction {0} , {1} , {2}",
			pParams->position.GetX(), pParams->position.GetY(), pParams->position.GetZ());
}

/// <summary>
/// Return point light at index
/// </summary>
std::weak_ptr<PointLight> SceneLightManager::GetPointLight(unsigned int index) const
{
	if (m_scenePointLights.size() != 0 && index < m_scenePointLights.size())
		return m_scenePointLights.at(index);
	else
		return {};
}

/// <summary>
/// Set the 3D position of a specified spot light
/// </summary>
void SceneLightManager::SetSpotLight(Vector3D position, unsigned int index)
{
	if (m_sceneSpotLights.at(index))
		m_sceneSpotLights.at(index)->m_position = position;
}

/// <summary>
/// Create a new spot light with specified starting data
/// </summary>
void SceneLightManager::AddSpotLight(std::shared_ptr<SpotLoaderParams> pParams)
{
	// Check available space
	if (m_sceneSpotLights.size() < m_maxSpotLights)
		m_sceneSpotLights.emplace_back(std::make_shared<SpotLight>(pParams));
	else
		PRINT_WARN("LIGHTMANAGER-> Can't create spot light at {0} , {1} , {2}",
			pParams->position.GetX(), pParams->position.GetY(), pParams->position.GetZ());
}

/// <summary>
/// Return spot light at index
/// </summary>
std::weak_ptr<SpotLight> SceneLightManager::GetSpotLight(unsigned int index) const
{
	if (m_sceneSpotLights.size() != 0 && index <= m_sceneSpotLights.size())
		return m_sceneSpotLights.at(index);
	else
		return {};
}
