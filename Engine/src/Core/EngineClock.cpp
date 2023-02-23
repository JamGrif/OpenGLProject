#include "pch.h"

#include "GLFW/glfw3.h"

constexpr static float ONE_SECOND = 1.0;

/// <summary>
/// Sets initial time
/// Done once on application start
/// </summary>
void EngineClock::Init()
{
	m_previousTime = glfwGetTime();
}

/// <summary>
/// Calculate applications delta time and FPS
/// Called every frame
/// </summary>
void EngineClock::Tick()
{
	// Delta time
	m_currentFrame = glfwGetTime();
	m_deltaTime = m_currentFrame - m_lastFrame;
	m_lastFrame = m_currentFrame;

	m_frameCount++;

	// If a second has passed.
	if (m_currentFrame - m_previousTime >= ONE_SECOND)
	{
		m_frameCountToDisplay = m_frameCount;
		m_frameCount = 0;

		m_previousTime = m_currentFrame;
	}
}

// -

static const char* EMPTY_TAG = "";

PerformanceTimer::PerformanceTimer(const char* tag)
	:m_tag(tag), m_bStopped(false)
{
	m_startTimepoint = std::chrono::high_resolution_clock::now();

	if (m_tag != EMPTY_TAG)
		PRINT_TRACE("{0} timer has started counting", m_tag);
	else
		PRINT_TRACE("A timer has started counting");
}

PerformanceTimer::~PerformanceTimer()
{
	if (!m_bStopped)
		Stop();
}

/// <summary>
/// Calculate duration and print out result to console
/// </summary>
void PerformanceTimer::Stop()
{
	m_bStopped = true;

	// Calculate timer duration
	const auto endTimepoint = std::chrono::high_resolution_clock::now();
	const uint64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
	const uint64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

	uint64_t duration = end - start;
	double ms = duration * 0.001;

	if (m_tag != EMPTY_TAG)
		PRINT_TRACE("{0} timer finished - duration is {1}ms", m_tag, ms);
	else
		PRINT_TRACE("A timer finished - duration is {0}ms", ms);
}
