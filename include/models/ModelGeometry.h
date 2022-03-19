#pragma once

#include "models/Model.h"

// Class that uses the geometry shader to simple "inflate/deflate" an object 
class ModelGeometry
	:public Model
{
public:

	ModelGeometry();
	~ModelGeometry();

	void		drawPassOne() override;
	void		drawPassTwo() override;

	void		setShaderTwo(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

private:

	std::shared_ptr<Texture>	m_geometryTexture;

	float						m_inflation;
	bool						m_increasing;

};

