#pragma once

#include <glm/vec3.hpp>

struct LightLoaderParams
{
	glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
	glm::vec3 diffuse{ 0.0f, 0.0f, 0.0f };
	glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
};

struct PointLoaderParams
	: public virtual LightLoaderParams
{
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
};

struct DirectionalLoaderParams
	: public virtual LightLoaderParams
{
	glm::vec3 direction{ 0.0f, 0.0f, 0.0f };
};

struct SpotLoaderParams
	: public PointLoaderParams, public DirectionalLoaderParams
{
};

// Base class for all 3 lights - provides basic variables they all use
struct BaseLight
{
	BaseLight(LightLoaderParams* pParams)
		:m_Ambient(pParams->ambient), m_Diffuse(pParams->diffuse), m_Specular(pParams->specular), m_lightActive(true)
	{
	}

	glm::vec3	m_Ambient;
	glm::vec3	m_Diffuse;
	glm::vec3	m_Specular;

	bool		m_lightActive; // Toggles whether entities should take lighting information from this light or not

protected:
	BaseLight() {}
};

struct PointLight : 
	public virtual BaseLight
{
	PointLight(PointLoaderParams* pParams)
		:BaseLight(pParams), m_Position(pParams->position), m_Constant(1.0f), m_Linear(0.07f), m_Quadratic(0.017f)
	{
		delete pParams;
	}

	glm::vec3	m_Position;

	// Point light attenuation
	float		m_Constant;
	float		m_Linear;
	float		m_Quadratic;

protected:
	PointLight() {}
};

struct DirectionalLight :
	public virtual BaseLight
{
	DirectionalLight(DirectionalLoaderParams* pParams)
		:BaseLight(pParams), m_Direction(pParams->direction)
	{
		delete pParams;
	}

	glm::vec3	m_Direction;

protected:
	DirectionalLight() {}
};

struct SpotLight :
	public PointLight, public DirectionalLight
{
	SpotLight(SpotLoaderParams* pParams)
		:BaseLight(pParams), m_cutOff(4.5f), m_outerCutOff(25.5f)
	{
		m_Position = pParams->position;
		m_Direction = pParams->direction;

		// Default spotlight attenuation
		m_Constant = 1.0f;
		m_Linear = 0.09f;
		m_Quadratic = 0.032f;

		delete pParams;
	}

	// Spot light spot edge
	const float	m_cutOff;
	const float	m_outerCutOff;
};

// Class allowing all 3 types of lights to be created and managed by providing utility functions to interact with a specified light
class SceneLightManager
{
public:
	SceneLightManager();
	~SceneLightManager();

	// Directional Lights
	void							setDirectionalLight(float x, float y, float z, int index = 0);
	void							addDirectionalLight(DirectionalLoaderParams* pParams);
	std::weak_ptr<DirectionalLight>	getDirectionalLight(int index = 0) const;

	unsigned int getCurrentDirectionalLights() const { return static_cast<unsigned int>(m_sceneDirectionalLights.size()); }

	// Point Lights
	void							setPointLight(float x, float y, float z, int index = 0);
	void							addPointLight(PointLoaderParams* pParams);
	std::weak_ptr<PointLight>		getPointLight(int index = 0) const;

	unsigned int getCurrentPointLights() const { return static_cast<unsigned int>(m_scenePointLights.size()); }

	// Spot Lights
	void							setSpotLight(float x, float y, float z, int index = 0);
	void							addSpotLight(SpotLoaderParams* pParams);
	std::weak_ptr<SpotLight>		getSpotLight(int index = 0) const;

	unsigned int getCurrentSpotLights() const{ return static_cast<unsigned int>(m_sceneSpotLights.size()); }

private:

	// Directional Lights
	std::vector<std::shared_ptr<DirectionalLight>>	m_sceneDirectionalLights;
	const unsigned int				m_maxDirectionalLights;

	// Point Lights
	std::vector<std::shared_ptr<PointLight>>		m_scenePointLights;
	const unsigned int				m_maxPointLights;

	// Spot Lights
	std::vector<std::shared_ptr<SpotLight>>			m_sceneSpotLights;
	const unsigned int				m_maxSpotLights;
};

