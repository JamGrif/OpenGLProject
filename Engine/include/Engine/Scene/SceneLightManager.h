#pragma once

#include <glm/vec3.hpp>

/// <summary>
/// Base class loading parameters which are passed to an ILight object on construction to set initial values
/// </summary>
struct ILightLoaderParams
{
	Vector3D ambient{ 0.0f, 0.0f, 0.0f };
	Vector3D diffuse{ 0.0f, 0.0f, 0.0f };
	Vector3D specular{ 0.0f, 0.0f, 0.0f };
};

/// <summary>
/// Loading parameters for a directional light
/// </summary>
struct DirectionalLoaderParams
	: public virtual ILightLoaderParams
{
	Vector3D direction{ 0.0f, 0.0f, 0.0f };
};

/// <summary>
/// Loading parameters for a point light
/// </summary>
struct PointLoaderParams
	: public virtual ILightLoaderParams
{
	Vector3D position{ 0.0f, 0.0f, 0.0f };
};

/// <summary>
/// Loading parameters for a spot light
/// Uses data from both DirectionalLoaderParams and PointLoaderParams
/// </summary>
struct SpotLoaderParams
	: public PointLoaderParams, public DirectionalLoaderParams
{
};

/// <summary>
/// Base class for all three light types
/// Directional - Point - Spot
/// </summary>
struct ILight
{
	ILight(std::shared_ptr<ILightLoaderParams> pParams)
		:m_ambient(pParams->ambient), m_diffuse(pParams->diffuse), m_specular(pParams->specular), m_bLightActive(true)
	{}
	~ILight()
	{}

	Vector3D	m_ambient{ 0,0,0 };
	Vector3D	m_diffuse{ 0,0,0 };
	Vector3D	m_specular{ 0,0,0 };

	bool		m_bLightActive; // Toggles whether entities should take lighting information from this light or not

protected:
	ILight() {}
};

struct PointLight : 
	public virtual ILight
{
	PointLight(std::shared_ptr<PointLoaderParams> pParams)
		:ILight(pParams), m_position(pParams->position), m_constant(1.0f), m_linear(0.07f), m_quadratic(0.017f)
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
	public virtual ILight
{
	DirectionalLight(std::shared_ptr<DirectionalLoaderParams> pParams)
		:ILight(pParams), m_direction(pParams->direction)
	{}

	Vector3D	m_direction{ 0,0,0 };

protected:
	DirectionalLight() {}
};

struct SpotLight :
	public PointLight, public DirectionalLight
{
	SpotLight(std::shared_ptr<SpotLoaderParams> pParams)
		:ILight(pParams), m_cutOff(4.5f), m_outerCutOff(25.5f)
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
/// Handles lifetime of lights and provides utility functions to use them
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

	uint8_t							GetCurrentDirectionalLights() const { return static_cast<uint8_t>(m_sceneDirectionalLights.size()); }

	// Point Lights
	void							SetPointLight(Vector3D position, unsigned int index);
	void							AddPointLight(std::shared_ptr<PointLoaderParams> pParams);
	std::weak_ptr<PointLight>		GetPointLight(unsigned int index) const;

	uint8_t							GetCurrentPointLights() const { return static_cast<uint8_t>(m_scenePointLights.size()); }

	// Spot Lights
	void							SetSpotLight(Vector3D position, unsigned int index);
	void							AddSpotLight(std::shared_ptr<SpotLoaderParams> pParams);
	std::weak_ptr<SpotLight>		GetSpotLight(unsigned int index) const;

	uint8_t							GetCurrentSpotLights() const{ return static_cast<uint8_t>(m_sceneSpotLights.size()); }

private:

	// Directional Lights
	typedef std::vector<std::shared_ptr<DirectionalLight>> SceneDirectionalLights;
	SceneDirectionalLights			m_sceneDirectionalLights;
	const unsigned int				m_maxDirectionalLights;

	// Point Lights
	typedef std::vector<std::shared_ptr<PointLight>> ScenePointLights;
	ScenePointLights				m_scenePointLights;
	const unsigned int				m_maxPointLights;

	// Spot Lights
	typedef std::vector<std::shared_ptr<SpotLight>> SceneSpotLights;
	SceneSpotLights					m_sceneSpotLights;
	const unsigned int				m_maxSpotLights;
};
