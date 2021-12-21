#pragma once
#include "Model.h"

//Class that is used to draw a simple sprite at a location
class ModelSprite :
    public Model
{
public:

    ModelSprite(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
    ~ModelSprite();

    void                    drawPassOne() override;
    void                    drawPassTwo() override;

    void                    setSprite(std::string filePath);


private:

    Texture*                m_modelSprite;

};

