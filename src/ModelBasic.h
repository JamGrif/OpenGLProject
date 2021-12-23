#pragma once

#include "Model.h"

//Class allows the drawing of a mesh with no texturing or lighting instead a solid colour. Also gives the option to "copy" the values of a specified point light
class ModelBasic :
    public Model
{
public:
    ModelBasic();
    ~ModelBasic();

    void        drawPassOne() override;
    void        drawPassTwo() override;

    void        copyPointLight(int index);

private:

    glm::vec3   m_defaultColour;

    const int   NOT_COPYING_LIGHT;

    int         m_pointLightToCopy;

};

