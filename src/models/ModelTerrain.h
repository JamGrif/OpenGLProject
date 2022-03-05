#pragma once

#include "Model.h"

//Class that uses tessellation shader to draw objects. Provides a way to alter the elevation of created terrain
class ModelTerrain :
    public Model
{
public:

    ModelTerrain();
    ~ModelTerrain();

    void        drawPassOne() override;
    void        drawPassTwo() override;

	void		setTerrainTexture(const std::string& texturePath);
	void		setTerrainHeightTexture(const std::string& texturePath);

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

