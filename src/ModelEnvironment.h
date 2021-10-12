#pragma once
#include "Model.h"

//Class allows meshes to have a reflection environment map or a refraction environment map from the skybox
class ModelEnvironment :
    public Model
{
public:
    ModelEnvironment(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
    ~ModelEnvironment();

    void        drawPassOne() override;
    void        drawPassTwo() override;

    void        toggleReflection(bool value);
    void        toggleRefraction(bool value);

private:

    CubeMap*    m_skyTexture;

    bool        m_usingReflection;
    bool        m_usingRefraction;

};

