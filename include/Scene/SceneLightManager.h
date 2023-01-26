#pragma once

#include <glm/vec3.hpp>

template <typename T>
class MiniLightManager;

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

// Base class for all 3 lights - provides basic variables they all use
struct BaseLight
{
	BaseLight(LightLoaderParams* pParams)
		:m_ambient(pParams->ambient), m_diffuse(pParams->diffuse), m_specular(pParams->specular), m_bLightActive(true)
	{
	}

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
	PointLight(PointLoaderParams* pParams)
		:BaseLight(pParams), m_position(pParams->position), m_constant(1.0f), m_linear(0.07f), m_quadratic(0.017f)
	{
		delete pParams;
	}

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
	DirectionalLight(DirectionalLoaderParams* pParams)
		:BaseLight(pParams), m_direction(pParams->direction)
	{
		delete pParams;
	}

	Vector3D	m_direction{ 0,0,0 };

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
	void							SetDirectionalLight(Vector3D position, int index = 0);
	void							AddDirectionalLight(DirectionalLoaderParams* pParams);
	std::weak_ptr<DirectionalLight>	GetDirectionalLight(int index = 0) const;

	inline unsigned int				GetCurrentDirectionalLights() const { return static_cast<unsigned int>(m_sceneDirectionalLights.size()); }

	// Point Lights
	void							SetPointLight(Vector3D position, int index = 0);
	void							AddPointLight(PointLoaderParams* pParams);
	std::weak_ptr<PointLight>		GetPointLight(int index = 0) const;

	inline unsigned int				GetCurrentPointLights() const { return static_cast<unsigned int>(m_scenePointLights.size()); }

	// Spot Lights
	void							SetSpotLight(Vector3D position, int index = 0);
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

	MiniLightManager<DirectionalLight>* m_miniDirManager;
};

template <typename T>
class MiniLightManager
{
public:
	MiniLightManager() {}
	~MiniLightManager() {}

	template <typename P>
	void AddLight(P* pParams)
	{
		// Ensure new Lights don't exceed the maximum amount allowed
		if (m_sceneLights.size() < m_maxLights)
		{
			m_sceneLights.emplace_back(std::make_shared<T>(pParams));
		}
	}

	std::weak_ptr<T>				GetLight(int index = 0)
	{
		// No directional lights exists
		if (m_sceneLights.size() == 0)
		{
			return {};
		}

		// Ensure index number is valid
		if (index <= m_sceneLights.size())
		{
			return m_sceneLights.at(index);
		}

		return {};
	}

	inline unsigned int				GetCurrentLights() const { return static_cast<unsigned int>(m_sceneLights.size()); }

private:

	std::vector<std::shared_ptr<T>> m_sceneLights;
	unsigned int				m_maxLights = 1;


};

