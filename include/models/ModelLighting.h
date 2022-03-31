#pragma once

#include "models/Model.h"

#include "CollisionObject.h"

// Class that allows an object to be lit by all the scene lights and gives the option for multiple texture maps to be applied
class ModelLighting :
    public Model
{
public:
    ModelLighting();
    ~ModelLighting();

	void					initModel() override;
	void					updateModel() override;
    void                    drawPassOne() override;
    void                    drawPassTwo() override;

    void                    setDiffuseTexture(const std::string& texturePath);
    void                    setSpecularTexture(const std::string& texturePath, float shininessAmount);
    void                    setEmissionTexture(const std::string& texturePath);
    void                    setNormalTexture(const std::string& texturePath, bool normalize);
    void                    setHeightTexture(const std::string& texturePath, float heightAmount);

	void					affectedByGravity(bool gravity);

private:

	std::shared_ptr<Texture>	m_modelTextures[5];
	bool						m_modelUsingTextures[5];

    float						m_specularShininess;

    bool						m_normalizeTexture;
    float						m_heightAmount;

    std::vector<glm::vec3>		m_tangents;
    std::vector<glm::vec3>		m_bitangents;

    enum textureSlotNumber
    {
        e_diffuseTextureSlot    = 0,
        e_specularTextureSlot   = 1,
        e_emissionTextureSlot   = 2,
        e_normalTextureSlot     = 3,
        e_heightTextureSlot     = 4
    };

	std::shared_ptr<CollisionObject> m_collisionObject;

};

