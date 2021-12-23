#pragma once
#include <glm\glm.hpp>
#include <vector>


//Base class for all 3 lights - provides utility functions they all can use
struct Light
{
	Light(glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f))
		:Ambient(ambient), Diffuse(diffuse), Specular(specular)
	{
		Ambient = ambient;
		Diffuse = diffuse;
		Specular = specular;
	}
	void toggleActive()
	{
		lightActive = lightActive == true ? false : true;
	}

	glm::vec3	Ambient;
	glm::vec3	Diffuse;
	glm::vec3	Specular;
	bool		lightActive = false;
};

struct PointLight
	:public Light
{
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
		:Light(ambient, diffuse, specular), Position(position)
	{
		lightActive = true;
	}

	glm::vec3	Position;

	// Point light attenuation
	float		Constant = 1.0f;
	float		Linear = 0.07f;
	float		Quadratic = 0.017f;
};

struct DirectionalLight
	:public Light
{
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f))
		:Light(ambient, diffuse, specular), Direction(direction)
	{
		lightActive = true;
	}

	glm::vec3	Direction;
};

struct SpotLight
	:public Light
{
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
		:Light(ambient, diffuse, specular), Position(position)
	{
		Direction.x = 0.0f;
		Direction.y = 0.0f;
		Direction.z = 0.0f;

		lightActive = true;
	}

	glm::vec3	Position;
	glm::vec3	Direction;

	// Spot light spot edge
	float		cutOff = 4.5f;
	float		outerCutOff = 25.5f;

	// Spot light attenuation
	float		Constant = 1.0f;
	float		Linear = 0.09f;
	float		Quadratic = 0.032f;
};

//Class allowing all 3 types of lights to be created and managed by providing utility functions to interact with a specified light
class LightManager
{
public:
	LightManager();
	~LightManager();

	//Directional Lights
	void							setDirectionalLight(float x, float y, float z, int index = 0);
	void							addDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
	DirectionalLight*				getDirectionalLight(int index = 0) const;

	int								getCurrentDirectionalLights() const;

	//Point Lights
	void							setPointLight(float x, float y, float z, int index = 0);
	void							addPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position);
	PointLight*						getPointLight(int index = 0) const;

	int								getCurrentPointLights() const;

	//Spot Lights
	void							setSpotLight(float x, float y, float z, int index = 0);
	void							addSpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position);
	SpotLight*						getSpotLight(int index = 0) const;

	int								getCurrentSpotLights() const;

private:

	//Directional Lights
	std::vector<DirectionalLight*>	m_sceneDirectionalLights;
	const int						m_maxDirectionalLights;
	int								m_currentDirectionalLights;


	//Point Lights
	std::vector<PointLight*>		m_scenePointLights;
	const int						m_maxPointLights;
	int								m_currentPointLights;


	//Spot Lights
	std::vector<SpotLight*>			m_sceneSpotLights;
	const int						m_maxSpotLights;
	int								m_currentSpotLights;
	

};

