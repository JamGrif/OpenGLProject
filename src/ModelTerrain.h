#pragma once

#include "Model.h"

//Class that uses tessellation shader to draw objects. Provides a way to alter the elevation of created terrain
class ModelTerrain :
    public Model
{
public:

    ModelTerrain(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), float elevation = 2.5);
    ~ModelTerrain();

    void        drawPassOne() override;
    void        drawPassTwo() override;

    void        setShaderTwo(const char* vertexPath, const char* tessellationControlPath, const char* tessellationEvaluationPath, const char* fragmentPath);

    void        setElevation(float value);
    void        alterElevation(float change);

private:

    Texture*    m_terrainTexture;
    Texture*    m_terrainHeight;

    float       m_elevation;

    const float m_minElevation;
    const float m_maxElevation;

};

