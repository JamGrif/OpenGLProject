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
		:m_ambient(pParams->ambient), m_diffuse(pParams->diffuse), m_specular(pParams->specular), m_bLightActive(true)
	{
	}

	glm::vec3	m_ambient;
	glm::vec3	m_diffuse;
	glm::vec3	m_specular;

	bool		m_bLightActive; // Toggles whether entities should take lighting information from this light or not

protected:
	BaseLight() {}
};

struct PointLight : 
	public virtual BaseLight
{
	PointLight(PointLoaderParams* pParams)
		:BaseLight(pParams), m_position(pParams->position), m_constant(1.0f), m_linear(0.07f), m_quadratic(0.017f)
	{
		delete pParams;
	}

	glm::vec3	m_position;

	// Point light attenuation
	float		m_constant;
	float		m_linear;
	float		m_quadratic;

protected:
	PointLight() {}
};

struct DirectionalLight :
	public virtual BaseLight
{
	DirectionalLight(DirectionalLoaderParams* pParams)
		:BaseLight(pParams), m_direction(pParams->direction)
	{
		delete pParams;
	}

	glm::vec3	m_direction;

protected:
	DirectionalLight() {}
};

struct SpotLight :
	public PointLight, public DirectionalLight
{
	SpotLight(SpotLoaderParams* pParams)
		:BaseLight(pParams), m_cutOff(4.5f), m_outerCutOff(25.5f)
	{
		m_position = pParams->position;
		m_direction = pParams->direction;

		// Default spotlight attenuation
		m_constant = 1.0f;
		m_linear = 0.09f;
		m_quadratic = 0.032f;

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
	void							SetDirectionalLight(float x, float y, float z, int index = 0);
	void							AddDirectionalLight(DirectionalLoaderParams* pParams);
	std::weak_ptr<DirectionalLight>	GetDirectionalLight(int index = 0) const;

	inline unsigned int				GetCurrentDirectionalLights() const { return static_cast<unsigned int>(m_sceneDirectionalLights.size()); }

	// Point Lights
	void							SetPointLight(float x, float y, float z, int index = 0);
	void							AddPointLight(PointLoaderParams* pParams);
	std::weak_ptr<PointLight>		GetPointLight(int index = 0) const;

	inline unsigned int				GetCurrentPointLights() const { return static_cast<unsigned int>(m_scenePointLights.size()); }

	// Spot Lights
	void							SetSpotLight(float x, float y, float z, int index = 0);
	void							AddSpotLight(SpotLoaderParams* pParams);
	std::weak_ptr<SpotLight>		GetSpotLight(int index = 0) const;

	inline unsigned int				GetCurrentSpotLights() const{ return static_cast<unsigned int>(m_sceneSpotLights.size()); }

private:

	// Directional Lights
	std::vector<std::shared_ptr<DirectionalLight>> m_sceneDirectionalLights;
	const unsigned int				m_maxDirectionalLights;

	// Point Lights
	std::vector<std::shared_ptr<PointLight>> m_scenePointLights;
	const unsigned int				m_maxPointLights;

	// Spot Lights
	std::vector<std::shared_ptr<SpotLight>> m_sceneSpotLights;
	const unsigned int				m_maxSpotLights;
};

