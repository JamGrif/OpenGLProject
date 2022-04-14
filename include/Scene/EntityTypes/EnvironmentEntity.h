#pragma once

#include "Scene/EntityTypes/BaseEntity.h"

struct templateEnvironmentEntity : public templateBaseEntity
{
	std::string mesh = "";

	bool reflection = false;

	bool refraction = false;
};


// Class allows meshes to have a reflection environment map or a refraction environment map from the Skybox
class EnvironmentEntity :
    public BaseEntity
{
public:
	EnvironmentEntity();
	EnvironmentEntity(templateEnvironmentEntity object);
    ~EnvironmentEntity();

	void		initEntity() override;
	void		updateEntity() override;
    void        drawPassOne() override;
    void        drawPassTwo() override;

    void        toggleReflection(bool value);
    void        toggleRefraction(bool value);

private:

	std::shared_ptr<OpenGLCubemap>    m_skyTexture;

    bool        m_usingReflection;
    bool        m_usingRefraction;

};

