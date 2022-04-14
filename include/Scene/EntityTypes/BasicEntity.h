#pragma once

#include "Scene/EntityTypes/BaseEntity.h"

// Forward Declarations
struct PointLight;

struct templateBasicEntity : public templateBaseEntity
{
	std::string mesh = "";

	int lightToCopy = -1;
};


// Class allows the drawing of a mesh with no texturing or lighting instead a solid colour. Also gives the option to "copy" the values of a specified point light
class BasicEntity :
    public BaseEntity
{
public:
	BasicEntity();
	BasicEntity(templateBasicEntity object);
    ~BasicEntity();

	void		initEntity() override;
	void		updateEntity() override;
    void        drawPassOne() override;
    void        drawPassTwo() override;

    void        copyPointLight(unsigned int index);

private:

    glm::vec3   m_defaultColour;

	bool		m_copyingLight;

	// Cached other classes
	PointLight* m_localPointLight; // Used if ModelBasic object is set to copy a pointlight in the scene
};

