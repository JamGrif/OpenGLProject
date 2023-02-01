#pragma once

#include <glm/vec3.hpp>

struct LightLoaderParams
{
	Vector3D ambient{ 0.0f, 0.0f, 0.0f };
	Vector3D diffuse{ 0.0f, 0.0f, 0.0f };
	Vector3D specular{ 0.0f, 0.0f, 0.0f };
};

struct PointLoaderParams
	: public virtual LightLoaderParams
{
	Vector3D position{ 0.0f, 0.0f, 0.0f };
};

struct DirectionalLoaderParams
	: public virtual LightLoaderParams
{
	Vector3D direction{ 0.0f, 0.0f, 0.0f };
};

struct SpotLoaderParams
	: public PointLoaderParams, public DirectionalLoaderParams
{
};

// Base class for all 3 light types
struct BaseLight
{
	BaseLight(std::shared_ptr<LightLoaderParams> pParams)
		:m_ambient(pParams->ambient), m_diffuse(pParams->diffuse), m_specular(pParams->specular), m_bLightActive(true)
	{}
	~BaseLight()
	{}

	Vector3D	m_ambient{ 0,0,0 };
	Vector3D	m_diffuse{ 0,0,0 };
	Vector3D	m_specular{ 0,0,0 };

	bool		m_bLightActive; // Toggles whether entities should take lighting information from this light or not

protected:
	BaseLight() {}
};

struct PointLight : 
	public virtual BaseLight
{
	PointLight(std::shared_ptr<PointLoaderParams> pParams)
		:BaseLight(pParams), m_position(pParams->position), m_constant(1.0f), m_linear(0.07f), m_quadratic(0.017f)
	{}

	Vector3D	m_position{ 0,0,0 };

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
	DirectionalLight(std::shared_ptr<DirectionalLoaderParams> pParams)
		:BaseLight(pParams), m_direction(pParams->direction)
	{}

	Vector3D	m_direction{ 0,0,0 };

protected:
	DirectionalLight() {}
};

struct SpotLight :
	public PointLight, public DirectionalLight
{
	SpotLight(std::shared_ptr<SpotLoaderParams> pParams)
		:BaseLight(pParams), m_cutOff(4.5f), m_outerCutOff(25.5f)
	{
		m_position = pParams->position;
		m_direction = pParams->direction;

		// Default spotlight attenuation
		m_constant = 1.0f;
		m_linear = 0.09f;
		m_quadratic = 0.032f;
	}

	// Spot light spot edge
	const float	m_cutOff;
	const float	m_outerCutOff;
};

/// <summary>
/// Manager for all three light types that can be present in scene
/// Handles creation and deletation of lights and provides utility functions to use them
/// Directional - Point - Spot light types
/// </summary>
class SceneLightManager
{
public:
	SceneLightManager();
	~SceneLightManager();

	// Directional Lights
	void							SetDirectionalLight(Vector3D direction, unsigned int index);
	void							AddDirectionalLight(std::shared_ptr<DirectionalLoaderParams> pParams);
	std::weak_ptr<DirectionalLight>	GetDirectionalLight(unsigned int index) const;

	unsigned int					GetCurrentDirectionalLights() const { return static_cast<unsigned int>(m_sceneDirectionalLights.size()); }

	// Point Lights
	void							SetPointLight(Vector3D position, unsigned int index);
	void							AddPointLight(std::shared_ptr<PointLoaderParams> pParams);
	std::weak_ptr<PointLight>		GetPointLight(unsigned int index) const;

	unsigned int					GetCurrentPointLights() const { return static_cast<unsigned int>(m_scenePointLights.size()); }

	// Spot Lights
	void							SetSpotLight(Vector3D position, unsigned int index);
	void							AddSpotLight(std::shared_ptr<SpotLoaderParams> pParams);
	std::weak_ptr<SpotLight>		GetSpotLight(unsigned int index) const;

	unsigned int					GetCurrentSpotLights() const{ return static_cast<unsigned int>(m_sceneSpotLights.size()); }

private:

	// Directional Lights
	SceneDirectionalLights			m_sceneDirectionalLights;
	const unsigned int				m_maxDirectionalLights;

	// Point Lights
	ScenePointLights				m_scenePointLights;
	const unsigned int				m_maxPointLights;

	// Spot Lights
	SceneSpotLights					m_sceneSpotLights;
	const unsigned int				m_maxSpotLights;
};
