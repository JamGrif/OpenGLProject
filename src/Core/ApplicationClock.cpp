#include "pch.h"

#include <GLFW/glfw3.h>

double		ApplicationClock::m_deltaTime = 0;
double		ApplicationClock::m_lastFrame = 0;

double		ApplicationClock::m_previousTime = 0;
int			ApplicationClock::m_frameCount = 0;
int			ApplicationClock::m_frameCountToDisplay = 0;
double		ApplicationClock::m_currentFrame = 0;

/// <summary>
/// Done once on program start
/// </summary>
void ApplicationClock::init()
{
	m_previousTime = glfwGetTime();
	m_frameCount = 0;
	m_currentFrame = 0;
}

/// <summary>
/// Called every frame
/// </summary>
void ApplicationClock::tick()
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

/// <summary>
/// Updated every .tick()
/// </summary>
/// <returns></returns>
double ApplicationClock::getDeltaTime()
{
	return m_deltaTime;
}

/// <summary>
/// Current FPS
/// Updated every .tick()
/// </summary>
int ApplicationClock::getFrameCount()
{
	return m_frameCountToDisplay;
}

// -

PerformanceTimer::PerformanceTimer(const char* tag)
	:m_tag(tag), m_stopped(false)
{
	m_startTimepoint = std::chrono::high_resolution_clock::now();

	if (m_tag != "")
	{
		PRINT_TRACE("{0} timer has started counting", m_tag);
	}
	else
	{
		PRINT_TRACE("A timer has started counting");
	}
}

PerformanceTimer::~PerformanceTimer()
{
	if (!m_stopped)
		stop();
}

/// <summary>
/// Guess what this does
/// </summary>
void PerformanceTimer::stop()
{
	m_stopped = true;

	auto endTimepoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

	auto duration = end - start;
	double ms = duration * 0.001;

	if (m_tag != "")
	{
		PRINT_TRACE("{0} timer finished - duration is {1}", m_tag, ms);
	}
	else
	{
		PRINT_TRACE("A timer finished - duration is {0}", ms);
	}
}
