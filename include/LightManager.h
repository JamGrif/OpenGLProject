#pragma once

#include <glm\glm.hpp>

// Base class for all 3 lights - provides basic variables they all use
struct Light
{
	Light(glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f))
		:Ambient(ambient), Diffuse(diffuse), Specular(specular), lightActive(true)
	{
	}

	glm::vec3	Ambient;
	glm::vec3	Diffuse;
	glm::vec3	Specular;

	bool		lightActive; // Toggles whether models should take lighting information from this light or not
};

struct PointLight : 
	public Light
{
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
		:Light(ambient, diffuse, specular), Position(position)
	{
	}

	glm::vec3	Position;

	// Point light attenuation
	const float		Constant = 1.0f;
	const float		Linear = 0.07f;
	const float		Quadratic = 0.017f;
};

struct DirectionalLight :
	public Light
{
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f))
		:Light(ambient, diffuse, specular), Direction(direction)
	{
	}

	glm::vec3	Direction;
};

struct SpotLight :
	public Light
{
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
		:Light(ambient, diffuse, specular), Position(position), Direction(0.0f, 0.0f, 0.0f)
	{
	}

	glm::vec3	Position;
	glm::vec3	Direction;

	// Spot light spot edge
	const float		cutOff = 4.5f;
	const float		outerCutOff = 25.5f;

	// Spot light attenuation
	const float		Constant = 1.0f;
	const float		Linear = 0.09f;
	const float		Quadratic = 0.032f;
};

// Class allowing all 3 types of lights to be created and managed by providing utility functions to interact with a specified light
class LightManager
{
public:
	LightManager();
	~LightManager();

	// Directional Lights
	void							setDirectionalLight(float x, float y, float z, int index = 0);
	void							addDirectionalLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& direction);
	DirectionalLight*				getDirectionalLight(int index = 0) const;

	unsigned int					getCurrentDirectionalLights() const;

	// Point Lights
	void							setPointLight(float x, float y, float z, int index = 0);
	void							addPointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position);
	PointLight*						getPointLight(int index = 0) const;

	unsigned int					getCurrentPointLights() const;

	// Spot Lights
	void							setSpotLight(float x, float y, float z, int index = 0);
	void							addSpotLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position);
	SpotLight*						getSpotLight(int index = 0) const;

	unsigned int					getCurrentSpotLights() const;

private:

	//Directional Lights
	std::vector<DirectionalLight*>	m_sceneDirectionalLights;
	const unsigned int				m_maxDirectionalLights;
	unsigned int					m_currentDirectionalLights;


	//Point Lights
	std::vector<PointLight*>		m_scenePointLights;
	const unsigned int				m_maxPointLights;
	unsigned int					m_currentPointLights;


	//Spot Lights
	std::vector<SpotLight*>			m_sceneSpotLights;
	const unsigned int				m_maxSpotLights;
	unsigned int					m_currentSpotLights;
	

};

