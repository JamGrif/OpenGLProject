#include "GameTimer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "EngineStatics.h"

GameTimer::GameTimer()
	:m_deltaTime(0), m_lastFrame(0), m_previousTime(0), m_frameCount(0), m_currentFrame(0)
{

}

GameTimer::~GameTimer()
{

}

void GameTimer::startGameTimer()
{
	m_previousTime = glfwGetTime();
	m_frameCount = 0;
	m_currentFrame = 0;
}

void GameTimer::stopGameTimer()
{

}

void GameTimer::updateGameTimer()
{
	// Delta time
	m_currentFrame = glfwGetTime();
	m_deltaTime = m_currentFrame - m_lastFrame;
	m_lastFrame = m_currentFrame;

	// Calculate framecount
	m_frameCount++;
	// If a second has passed.
	if (m_currentFrame - m_previousTime >= 1.0)
	{
		// Display the frame count here any way you want.
		//m_appWindow->setWindowTitle(std::to_string(m_frameCount));

		m_frameCount = 0;
		m_previousTime = m_currentFrame;
	}
	EngineStatics::setDeltaTime(m_deltaTime);
}

double GameTimer::getDeltaTime()
{
	return m_deltaTime;
}
