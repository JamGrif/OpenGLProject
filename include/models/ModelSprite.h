#pragma once
#include "models/Model.h"

//Class that is used to draw a simple sprite at a location
class ModelSprite :
    public Model
{
public:
    ModelSprite();
    ~ModelSprite();

    void                    drawPassOne() override;
    void                    drawPassTwo() override;

    void                    setSprite(const std::string& filePath);

private:

	std::shared_ptr<Texture>	m_modelSprite;
};

