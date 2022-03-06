#include "models/ModelSprite.h"

ModelSprite::ModelSprite()
	:m_modelSprite(nullptr)
{
	setMesh("res/meshes/grass.obj");

	setShaderTwo("res/shaders/blendingSprite-vertex.glsl", "res/shaders/blendingSprite-fragment.glsl");
}



ModelSprite::~ModelSprite()
{
	if (m_modelSprite)
	{
		m_modelSprite = nullptr;
	}
}

/// <summary>
/// Overridden method from Model base class - Unused in this class
/// </summary>
void ModelSprite::drawPassOne()
{
	////If no valid model or shader attached
	//if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	//{
	//	return;
	//}
}

/// <summary>
/// Overridden method from Model base class - Used to draw the sprite in the scene
/// </summary>
void ModelSprite::drawPassTwo()
{
	// If no valid model or shader attached
	if (!m_modelMesh || !m_modelShaderPassTwo)
	{
		return;
	}

	// Bind shader
	m_modelShaderPassTwo->Bind();

	/*
		Set Vertex values
	*/
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", m_localProjectionMatrix);
	m_modelShaderPassTwo->setUniform1i("sprite", 0);

	m_modelSprite->Bind(0);

	/*
		Bind VBOs and vertex attributes
	*/

	setVBOAttrib(true, false, true, false, false);

	/*
		Draw
	*/

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_modelMesh->getIndices().size()), GL_UNSIGNED_INT, 0);

	/*
		Post-draw cleanup
	*/

	m_modelSprite->Unbind();
}

void ModelSprite::setSprite(const std::string& filePath)
{
	m_modelSprite = TextureManager::retrieveTexture(filePath);
}


