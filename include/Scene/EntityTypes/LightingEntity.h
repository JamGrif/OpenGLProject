#pragma once

#include "Scene/EntityTypes/BaseEntity.h"

struct templateLightingEntity : public templateBaseEntity
{
	std::string mesh = "";

	std::string diffuseMap = "";

	std::string specularMap = "";

	std::string normalMap = "";
	bool normalMapNormalize = false;

	std::string heightMap = "";
	float heightMapHeight = 0.0f;

	std::string emissionMap = "";

	bool gravity = false;
};

enum textureSlotNumber
{
	e_diffuseTextureSlot = 0,
	e_specularTextureSlot = 1,
	e_emissionTextureSlot = 2,
	e_normalTextureSlot = 3,
	e_heightTextureSlot = 4
};

// Class that allows an object to be lit by all the scene lights and gives the option for multiple texture maps to be applied
class LightingEntity :
    public BaseEntity
{
public:
	LightingEntity(templateLightingEntity object);
    ~LightingEntity();

	void							initEntity() override;
	void							updateEntity() override;
	void							drawPassOne() override;
	void							drawPassTwo() override;

	void							setDiffuseTexture(const std::string& texturePath);
	void							setSpecularTexture(const std::string& texturePath, float shininessAmount);
	void							setEmissionTexture(const std::string& texturePath);
	void							setNormalTexture(const std::string& texturePath, bool normalize);
    void							setHeightTexture(const std::string& texturePath, float heightAmount);

	// Editor
	std::shared_ptr<OpenGLMesh>		getMesh();
	std::shared_ptr<OpenGLTexture>	getTextureAtSlot(unsigned int index);
	std::size_t						getTextureAmount();

private:

	std::vector<std::shared_ptr<OpenGLTexture>> m_modelTextures;
	bool							m_modelUsingTextures[5];

    float							m_specularShininess;

    bool							m_normalizeTexture;
    float							m_heightAmount;

};

