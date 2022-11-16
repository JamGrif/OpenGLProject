#include "pch.h"
#include "Scene/SceneLightManager.h"


SceneLightManager::SceneLightManager()
	:m_maxDirectionalLights(1),
	m_maxPointLights(4),
	m_maxSpotLights(1)
{
}

SceneLightManager::~SceneLightManager()
{
	m_scenePointLights.clear();

	m_sceneDirectionalLights.clear();

	m_sceneSpotLights.clear();

	//EngineStatics::setLightManager(nullptr);
}


/// <summary>
/// Sets the XYZ direction of a specified directional light
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="index">Index of the light within the directional light vector</param>
void SceneLightManager::setDirectionalLight(float x, float y, float z, int index)
{
	if (m_sceneDirectionalLights.at(index))
	{
		m_sceneDirectionalLights.at(index)->m_Direction.x = x;
		m_sceneDirectionalLights.at(index)->m_Direction.y = y;
		m_sceneDirectionalLights.at(index)->m_Direction.z = z;
	}
}


void SceneLightManager::addDirectionalLight(DirectionalLoaderParams* pParams)
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
std::weak_ptr<DirectionalLight> SceneLightManager::getDirectionalLight(int index) const
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
void SceneLightManager::setPointLight(float x, float y, float z, int index)
{
	if (m_scenePointLights.at(index))
	{
		m_scenePointLights.at(index)->m_Position.x = x;
		m_scenePointLights.at(index)->m_Position.y = y;
		m_scenePointLights.at(index)->m_Position.z = z;
	}
}


void SceneLightManager::addPointLight(PointLoaderParams* pParams)
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
std::weak_ptr<PointLight> SceneLightManager::getPointLight(int index) const
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
void SceneLightManager::setSpotLight(float x, float y, float z, int index)
{
	if (m_sceneSpotLights.at(index))
	{
		m_sceneSpotLights.at(index)->m_Position.x = x;
		m_sceneSpotLights.at(index)->m_Position.y = y;
		m_sceneSpotLights.at(index)->m_Position.z = z;
	}
}


void SceneLightManager::addSpotLight(SpotLoaderParams* pParams)
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
std::weak_ptr<SpotLight> SceneLightManager::getSpotLight(int index) const
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





