#include "pch.h"
#include "Scene/SceneLightManager.h"

#include "Core/EngineStatics.h"

SceneLightManager::SceneLightManager()
	:m_maxDirectionalLights(1), m_currentDirectionalLights(0),
	m_maxPointLights(4), m_currentPointLights(0),
	m_maxSpotLights(1), m_currentSpotLights(0)
{
}

SceneLightManager::~SceneLightManager()
{
	for (PointLight* pl : m_scenePointLights)
	{
		delete pl;
		pl = nullptr;
	}
	m_scenePointLights.clear();

	for (DirectionalLight* dl : m_sceneDirectionalLights)
	{
		delete dl; 
		dl = nullptr;
	}
	m_sceneDirectionalLights.clear();

	for (SpotLight* sl : m_sceneSpotLights)
	{
		delete sl;
		sl = nullptr;
	}
	m_sceneSpotLights.clear();

	EngineStatics::setLightManager(nullptr);
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
	if (m_sceneDirectionalLights.at(index) != nullptr)
	{
		m_sceneDirectionalLights.at(index)->Direction.x = x;
		m_sceneDirectionalLights.at(index)->Direction.y = y;
		m_sceneDirectionalLights.at(index)->Direction.z = z;
	}
}

/// <summary>
/// Creates a new directional light if max limit has not been reached
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void SceneLightManager::addDirectionalLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& direction)
{
	// Ensure new directional lights don't exceed the maximum amount allowed
	if (m_currentDirectionalLights < m_maxDirectionalLights)
	{
		DirectionalLight* dl = new DirectionalLight(ambient, diffuse, specular, direction);
		m_sceneDirectionalLights.push_back(dl);
		m_currentDirectionalLights++;
	}
	else
	{
		PRINT_WARN("LIGHTMANAGER-> Can't create new directional light with direction {0} , {1} , {2}", direction.x, direction.y, direction.z);
	}
}

/// <summary>
/// Gets a specified directional light from the vector
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
DirectionalLight* SceneLightManager::getDirectionalLight(int index) const
{
	// No directional lights exists
	if (m_currentDirectionalLights == 0)
	{
		return nullptr;
	}

	// Ensure index number is valid
	if (index <= m_sceneDirectionalLights.size())
	{
		return m_sceneDirectionalLights.at(index);
	}
	 
	return nullptr;
	
}

/// <summary>
/// Returns the amount of directional lights active in scene
/// </summary>
/// <returns></returns>
unsigned int SceneLightManager::getCurrentDirectionalLights() const
{
	return m_currentDirectionalLights;
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
	if (m_scenePointLights.at(index) != nullptr)
	{
		m_scenePointLights.at(index)->Position.x = x;
		m_scenePointLights.at(index)->Position.y = y;
		m_scenePointLights.at(index)->Position.z = z;
	}
}

/// <summary>
/// Creates a new point light if max limit has not been reached
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void SceneLightManager::addPointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position)
{
	//Ensure new point lights don't exceed the maximum amount allowed
	if (m_currentPointLights < m_maxPointLights)
	{
		PointLight* point = new PointLight(ambient, diffuse, specular, position);
		m_scenePointLights.push_back(point);
		m_currentPointLights++;
	}
	else
	{
		PRINT_WARN("LIGHTMANAGER-> Can't create new point light at position {0} , {1} , {2}", position.x, position.y, position.z);
	}
}

/// <summary>
/// Gets a specified directional light from the vector
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
PointLight* SceneLightManager::getPointLight(int index) const
{
	//No point lights exists
	if (m_currentPointLights == 0)
	{
		return nullptr;
	}

	//Ensure index number is valid
	if (index < m_scenePointLights.size())
	{
		return m_scenePointLights.at(index);
	}

	return nullptr;
	
}

/// <summary>
/// Returns the amount of point lights active in scene
/// </summary>
/// <returns></returns>
unsigned int SceneLightManager::getCurrentPointLights() const
{
	return m_currentPointLights;
}

/// <summary>
/// Sets the XYZ position of a specified spot light
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="index">Index of the light within the spot light vector</param>
void SceneLightManager::setSpotLight(float x, float y, float z, int index)
{
	if (m_sceneSpotLights.at(index) != nullptr)
	{
		m_sceneSpotLights.at(index)->Position.x = x;
		m_sceneSpotLights.at(index)->Position.y = y;
		m_sceneSpotLights.at(index)->Position.z = z;
	}
}

/// <summary>
/// Creates a new spot light if max limit has not been reached
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void SceneLightManager::addSpotLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position)
{
	//Ensure new spot lights don't exceed the maximum amount allowed
	if (m_currentSpotLights < m_maxSpotLights)
	{
		SpotLight* spot = new SpotLight(ambient, diffuse, specular, position);
		m_sceneSpotLights.push_back(spot);
		m_currentSpotLights++;
	}
	else
	{
		PRINT_WARN("LIGHTMANAGER-> Can't create spot light at {0} , {1} , {2}", position.x, position.y, position.z);
	}
}

/// <summary>
/// Gets a specified spot light from the vector
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
SpotLight* SceneLightManager::getSpotLight(int index) const
{
	// No point lights exists
	if (m_currentSpotLights == 0)
	{
		return nullptr;
	}

	// Ensure index number is valid
	if (index <= m_sceneSpotLights.size())
	{
		return m_sceneSpotLights.at(index);
	}

	return nullptr;
}

/// <summary>
/// Returns the amount of spot lights active in scene
/// </summary>
/// <returns></returns>
unsigned int SceneLightManager::getCurrentSpotLights() const
{
	return m_currentSpotLights;
}





