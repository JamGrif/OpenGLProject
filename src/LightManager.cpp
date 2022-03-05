#include "LightManager.h"

#include "EngineStatics.h"

#include <iostream>

LightManager::LightManager()
	:m_maxDirectionalLights(1), m_currentDirectionalLights(0),
	m_maxPointLights(4), m_currentPointLights(0),
	m_maxSpotLights(1), m_currentSpotLights(0)
{
	std::cout << "LightManager Initialized" << std::endl;

	EngineStatics::setLightManager(this);
}

LightManager::~LightManager()
{
	std::cout << "LightManager Destroyed" << std::endl;

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
void LightManager::setDirectionalLight(float x, float y, float z, int index)
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
void LightManager::addDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
{
	// Ensure new directional lights don't exceed the maximum amount allowed
	if (m_currentDirectionalLights < m_maxDirectionalLights)
	{
		DirectionalLight* dl = new DirectionalLight(ambient, diffuse, specular, direction);
		m_sceneDirectionalLights.push_back(dl);
		m_currentDirectionalLights++;

		std::cout << "LIGHTMANAGER->New directional light created with direction " << direction.x << " " << direction.y << " " << direction.z << std::endl;
	}
}

/// <summary>
/// Gets a specified directional light from the vector
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
DirectionalLight* LightManager::getDirectionalLight(int index) const
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
unsigned int LightManager::getCurrentDirectionalLights() const
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
void LightManager::setPointLight(float x, float y, float z, int index)
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
void LightManager::addPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position)
{
	//Ensure new point lights don't exceed the maximum amount allowed
	if (m_currentPointLights < m_maxPointLights)
	{
		PointLight* point = new PointLight(ambient, diffuse, specular, position);
		m_scenePointLights.push_back(point);
		m_currentPointLights++;
		
		std::cout << "LIGHTMANAGER->New point light created at " << position.x << " " << position.y << " " << position.z << std::endl;
	}
}

/// <summary>
/// Gets a specified directional light from the vector
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
PointLight* LightManager::getPointLight(int index) const
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
unsigned int LightManager::getCurrentPointLights() const
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
void LightManager::setSpotLight(float x, float y, float z, int index)
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
void LightManager::addSpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position)
{
	//Ensure new point lights don't exceed the maximum amount allowed
	if (m_currentSpotLights < m_maxSpotLights)
	{
		SpotLight* spot = new SpotLight(ambient, diffuse, specular, position);
		m_sceneSpotLights.push_back(spot);
		m_currentSpotLights++;

		std::cout << "LIGHTMANAGER->New spot light created at " << position.x << " " << position.y << " " << position.z << std::endl;
	}
}

/// <summary>
/// Gets a specified spot light from the vector
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
SpotLight* LightManager::getSpotLight(int index) const
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
unsigned int LightManager::getCurrentSpotLights() const
{
	return m_currentSpotLights;
}





