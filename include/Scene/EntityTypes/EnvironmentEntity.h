#pragma once

#include "Scene/EntityTypes/BaseEntity.h"

// Class allows meshes to have a reflection environment map or a refraction environment map from the skybox
class EnvironmentEntity :
    public BaseEntity
{
public:
	EnvironmentEntity();
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

