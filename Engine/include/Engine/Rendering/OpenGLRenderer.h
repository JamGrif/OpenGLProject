#pragma once

#include <glm/mat4x4.hpp>

// Required for ScreenFilters enum
#include "Rendering/OpenGLFramebuffer.h"

/// <summary>
/// Interface for rendering buffers that are bound to the OpenGL context
/// </summary>
class OpenGLRenderer
{
public:

	bool				Init();
	bool				Clean();

	const glm::mat4&	GetProjectionMatrix() { return m_projMatrix; }

	void				StartOfFrame() const;
	void				EndOfFrame() const;

	void				Draw(size_t indicesCount) const;
	void				DrawCubemap(size_t vertexCount) const;
	void				DrawTerrain(size_t vertexCount) const;

	void				SetScreenFilter(ScreenFilter newFilter);

	static OpenGLRenderer* Get() // Get instance
	{
		static OpenGLRenderer* s_pInstance = new OpenGLRenderer;
		return s_pInstance;
	}

private:
	OpenGLRenderer();
	~OpenGLRenderer();

	glm::mat4			m_projMatrix;

	OpenGLIndex			m_appVAO;

	std::unique_ptr<OpenGLFramebuffer>	m_pSceneMSAAFrameBuffer;	// Scene is drawn to this buffer with MSAA applied
	std::unique_ptr<OpenGLFramebuffer>	m_pSceneFilterFramebuffer;	// Receives info from the MSAAframebuffer which then draws onto a quad which gets displayed to the screen
};
typedef OpenGLRenderer TheOpenGLRenderer;
