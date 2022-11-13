#pragma once

#include <glm/mat4x4.hpp>

class OpenGLRenderer
{
public:

	bool				init();
	bool				clean();

	void				startOfFrame() const;
	void				swapBuffers() const;

	void				draw(size_t indicesCount) const;
	void				drawCubemap(size_t vertexCount) const;
	void				drawTerrain(size_t vertexCount) const;

	static OpenGLRenderer* Instance()
	{
		static OpenGLRenderer* s_pInstance = new OpenGLRenderer;
		return s_pInstance;
	}

private:
	OpenGLRenderer();
	~OpenGLRenderer();

	glm::mat4			m_projMatrix;

	uint32_t			m_appVAO;
};
typedef OpenGLRenderer TheOpenGLRenderer;
