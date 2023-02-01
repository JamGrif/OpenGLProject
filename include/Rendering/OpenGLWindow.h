#pragma once


/// <summary>
/// A wrapper class around the GLFWwindow variable, providing a way to create and destroy one, 
/// along with utility functions and a way to check its status.
/// </summary>
class OpenGLWindow
{
public:

	bool		Init(int windowWidth, int windowHeight, const std::string& windowTitle, bool fullScreen);
	bool		Clean();

	GLFWwindow* GetGLFWWindow() const { return m_pWindow; }

	bool		ShouldClose() const;

	void		SetWindowIcon(const std::string& iconPath) const;
	void		SetWindowTitle(const std::string& newTitle) const;

	int			GetWindowWidth() const { return m_currentWindowWidth; }
	int			GetWindowHeight() const { return m_currentWindowHeight; }
	float		GetAspectRatio() const { return m_aspectRatio; }

	static OpenGLWindow* Get() // Get instance
	{
		static OpenGLWindow* s_pInstance = new OpenGLWindow;
		return s_pInstance;
	}

private:
	OpenGLWindow() :m_currentWindowWidth(0), m_currentWindowHeight(0), m_aspectRatio(0), m_pWindow(nullptr) {}
	~OpenGLWindow() {}

	uint32_t	m_currentWindowWidth;
	uint32_t	m_currentWindowHeight;
	float		m_aspectRatio;

	GLFWwindow* m_pWindow;

};
typedef OpenGLWindow TheOpenGLWindow;
