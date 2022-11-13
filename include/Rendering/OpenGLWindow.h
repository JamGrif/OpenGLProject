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

	bool		init(int windowWidth, int windowHeight, const std::string& windowTitle, bool fullScreen);
	bool		clean();

	GLFWwindow* getWindowPtr() const;

	bool		shouldClose() const;

	void		setWindowIcon(const std::string& iconPath) const;
	void		setWindowTitle(const std::string& newTitle) const;

	int			getWindowWidth() const;
	int			getWindowHeight() const;
	float		getAspectRatio() const;

	static OpenGLWindow* Instance()
	{
		static OpenGLWindow* s_pInstance = new OpenGLWindow;
		return s_pInstance;
	}

private:
	OpenGLWindow() :m_currentWindowWidth(0), m_currentWindowHeight(0), m_aspectRatio(0), m_window(nullptr) {};
	~OpenGLWindow() {};

	uint32_t	m_currentWindowWidth;
	uint32_t	m_currentWindowHeight;
	float		m_aspectRatio;

	GLFWwindow* m_window;

};
typedef OpenGLWindow TheOpenGLWindow;
