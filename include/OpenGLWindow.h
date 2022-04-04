#pragma once

// Forward Declarations
struct GLFWwindow;

/// <summary>
/// A wrapper class around the GLFWwindow variable, providing a way to create and destroy one, 
/// along with utility functions and a way to check its status.
/// </summary>
class OpenGLWindow
{
public:
	OpenGLWindow(int windowWidth, int windowHeight, const std::string& windowTitle, bool fullScreen);
	~OpenGLWindow();

	GLFWwindow* getWindow() const;

	void		setWindowIcon(const std::string& iconPath) const;
	void		setWindowTitle(const std::string& newTitle) const;

	int			getWindowWidth() const;
	int			getWindowHeight() const;
	float		getAspectRatio() const;
	bool		getWindowStatus() const;

private:

	bool		m_status;

	int			m_currentWindowWidth;
	int			m_currentWindowHeight;
	float		m_aspectRatio;

	GLFWwindow* m_Window;

};
