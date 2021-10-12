#pragma once

#include "Model.h"

//Class that uses the geometry shader to simple "inflate/deflate" an object 
class ModelGeometry
	:public Model
{
public:

	ModelGeometry(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	~ModelGeometry();

	void		drawPassOne() override;
	void		drawPassTwo() override;

	void		setShaderTwo(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

private:

	Texture*	m_GeometryTexture;

	float		m_inflation;
	bool		m_increasing;

};

