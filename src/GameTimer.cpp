#include "GameTimer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "EngineStatics.h"

GameTimer::GameTimer()
	:m_deltaTime(0), m_lastFrame(0), m_previousTime(0), m_frameCount(0), m_frameCountToDisplay(0), m_currentFrame(0)
{
	//EngineStatics::setGameTimer(this);
}

GameTimer::~GameTimer()
{
	EngineStatics::setGameTimer(nullptr);
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
		m_frameCountToDisplay = m_frameCount;
		m_frameCount = 0;
		m_previousTime = m_currentFrame;
	}
}

double GameTimer::getDeltaTime() const
{
	return m_deltaTime;
}

int GameTimer::getFrameCount() const
{
	return m_frameCountToDisplay;
}
