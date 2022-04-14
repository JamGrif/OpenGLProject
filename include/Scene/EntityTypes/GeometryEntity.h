#pragma once

#include "Scene/EntityTypes/BaseEntity.h"

struct templateGeometryEntity : public templateBaseEntity
{
	std::string mesh = "";
};

// Class that uses the geometry shader to simple "inflate/deflate" an object 
class GeometryEntity
	:public BaseEntity
{
public:
	GeometryEntity();
	GeometryEntity(templateGeometryEntity object);
	~GeometryEntity();

	void		initEntity() override;
	void		updateEntity() override;
	void		drawPassOne() override;
	void		drawPassTwo() override;

	void		setShaderTwo(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

private:

	std::shared_ptr<OpenGLTexture>	m_geometryTexture;

	float						m_inflation;
	bool						m_increasing;

};

