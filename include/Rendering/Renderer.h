#pragma once

#include <GL/glew.h> // glew.h must always be the first openGL header to be included (before glfw3.h)
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

// Forward Declarations
class OpenGLWindow;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void startOfFrame() const;
	void swapBuffers() const;

	void draw(size_t indicesCount) const;
	void drawCubemap(size_t vertexCount) const;
	void drawTerrain(size_t vertexCount) const;

	bool getStatus();
private:
	bool m_status;

	std::shared_ptr<OpenGLWindow>	m_appWindow;

	glm::mat4						m_projMatrix;

	uint32_t							m_appVAO;

};

