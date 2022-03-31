#pragma once

#include "models/Model.h"

// Forward Declarations
struct PointLight;

// Class allows the drawing of a mesh with no texturing or lighting instead a solid colour. Also gives the option to "copy" the values of a specified point light
class ModelBasic :
    public Model
{
public:
    ModelBasic();
    ~ModelBasic();

	void		initModel() override;
	void		updateModel() override;
    void        drawPassOne() override;
    void        drawPassTwo() override;

    void        copyPointLight(unsigned int index);

private:

    glm::vec3   m_defaultColour;

	bool		m_copyingLight;

	// Cached other classes
	PointLight* m_localPointLight; // Used if ModelBasic object is set to copy a pointlight in the scene
};

