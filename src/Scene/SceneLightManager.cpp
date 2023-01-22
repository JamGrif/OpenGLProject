#include "pch.h"
#include "Scene/SceneLightManager.h"

static constexpr int DEFAULT_DIRECTIONAL_LIGHT_AMOUNT = 1;
static constexpr int DEFAULT_POINT_LIGHT_AMOUNT = 4;
static constexpr int DEFAULT_SPOT_LIGHT_AMOUNT = 1;

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
/// Sets the XYZ direction of a specified directional light
/// </summary>
void SceneLightManager::SetDirectionalLight(float x, float y, float z, int index)
{
	if (m_sceneDirectionalLights.at(index))
	{
		m_sceneDirectionalLights.at(index)->m_direction.x = x;
		m_sceneDirectionalLights.at(index)->m_direction.y = y;
		m_sceneDirectionalLights.at(index)->m_direction.z = z;
	}
}

void SceneLightManager::AddDirectionalLight(DirectionalLoaderParams* pParams)
{
	// Ensure new directional lights don't exceed the maximum amount allowed
	if (m_sceneDirectionalLights.size() < m_maxDirectionalLights)
	{
		m_sceneDirectionalLights.emplace_back(std::make_shared<DirectionalLight>(pParams));
	}
	else
	{
		PRINT_WARN("LIGHTMANAGER-> Can't create new directional light with direction {0} , {1} , {2}", pParams->direction.x, pParams->direction.y, pParams->direction.z);
	}
}

/// <summary>
/// Gets a specified directional light from the vector
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
std::weak_ptr<DirectionalLight> SceneLightManager::GetDirectionalLight(int index) const
{
	// No directional lights exists
	if (m_sceneDirectionalLights.size() == 0)
	{
		return {};
	}

	// Ensure index number is valid
	if (index <= m_sceneDirectionalLights.size())
	{
		return m_sceneDirectionalLights.at(index);
	}
	 
	return {};
	
}

/// <summary>
/// Sets the XYZ position of a specified point light
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="index">Index of the light within the point light vector</param>
void SceneLightManager::SetPointLight(float x, float y, float z, int index)
{
	if (m_scenePointLights.at(index))
	{
		m_scenePointLights.at(index)->m_position.x = x;
		m_scenePointLights.at(index)->m_position.y = y;
		m_scenePointLights.at(index)->m_position.z = z;
	}
}


void SceneLightManager::AddPointLight(PointLoaderParams* pParams)
{
	// Ensure new point lights don't exceed the maximum amount allowed
	if (m_scenePointLights.size() < m_maxPointLights)
	{
		m_scenePointLights.emplace_back(std::make_shared<PointLight>(pParams));
	}
	else
	{
		PRINT_WARN("LIGHTMANAGER-> Can't create new point light with direction {0} , {1} , {2}", pParams->position.x, pParams->position.y, pParams->position.z);
	}
}

/// <summary>
/// Gets a specified directional light from the vector
/// </summary>
std::weak_ptr<PointLight> SceneLightManager::GetPointLight(int index) const
{
	//No point lights exists
	if (m_scenePointLights.size() == 0)
	{
		return {};
	}

	//Ensure index number is valid
	if (index < m_scenePointLights.size())
	{
		return m_scenePointLights.at(index);
	}

	return {};
}

/// <summary>
/// Set the position of a specified spot light
void SceneLightManager::SetSpotLight(float x, float y, float z, int index)
{
	if (m_sceneSpotLights.at(index))
	{
		m_sceneSpotLights.at(index)->m_position.x = x;
		m_sceneSpotLights.at(index)->m_position.y = y;
		m_sceneSpotLights.at(index)->m_position.z = z;
	}
}


void SceneLightManager::AddSpotLight(SpotLoaderParams* pParams)
{
	//Ensure new spot lights don't exceed the maximum amount allowed
	if (m_sceneSpotLights.size() < m_maxSpotLights)
	{
		m_sceneSpotLights.emplace_back(std::make_shared<SpotLight>(pParams));
	}
	else
	{
		PRINT_WARN("LIGHTMANAGER-> Can't create spot light at {0} , {1} , {2}", pParams->position.x, pParams->position.y, pParams->position.z);
	}
}

/// <summary>
/// Gets a specified spot light
/// </summary>
std::weak_ptr<SpotLight> SceneLightManager::GetSpotLight(int index) const
{
	// No point lights exists
	if (m_sceneSpotLights.size() == 0)
	{
		return {};
	}

	// Ensure index number is valid
	if (index <= m_sceneSpotLights.size())
	{
		return m_sceneSpotLights.at(index);
	}

	return {};
}





