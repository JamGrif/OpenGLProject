#include "ModelSprite.h"

ModelSprite::ModelSprite(glm::vec3 position, glm::vec3 rotation)
	:Model(position, glm::vec3(90,0,90)), m_modelSprite(nullptr)
{
	
	setMesh("res/meshes/grass.obj");

	setShaderTwo("res/shaders/blendingSprite-vertex.glsl", "res/shaders/blendingSprite-fragment.glsl");
}



ModelSprite::~ModelSprite()
{
	m_modelSprite = nullptr;
}

/// <summary>
/// Overriden method from Model base class - Unused in this class
/// </summary>
void ModelSprite::drawPassOne()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

/// <summary>
/// Overriden method from Model base class - Used to draw the sprite in the scene
/// </summary>
void ModelSprite::drawPassTwo()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	m_modelShaderPassTwo->Bind();

	//Set Vertex values
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());

	m_modelShaderPassTwo->setUniform1i("sprite", 0);
	m_modelSprite->Bind(0);

	setVBOAttrib(true, false, true, false, false);

	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	m_modelSprite->Unbind();


}

void ModelSprite::setSprite(const char* filePath)
{
	m_modelSprite = TextureManager::retrieveTexture(filePath);
}


