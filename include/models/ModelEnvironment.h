#pragma once
#include "models/Model.h"

// Class allows meshes to have a reflection environment map or a refraction environment map from the skybox
class ModelEnvironment :
    public Model
{
public:
    ModelEnvironment();
    ~ModelEnvironment();

	void		updateModel() override;
    void        drawPassOne() override;
    void        drawPassTwo() override;

    void        toggleReflection(bool value);
    void        toggleRefraction(bool value);

private:

	std::shared_ptr<Cubemap>    m_skyTexture;

    bool        m_usingReflection;
    bool        m_usingRefraction;

};

