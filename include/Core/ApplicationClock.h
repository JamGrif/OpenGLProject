#pragma once

#include <chrono>

class ApplicationClock
{
public:

	static void		Init();
	static void		Tick();

	static double	GetDeltaTime() { return m_deltaTime; }
	static int		GetFrameCount() { return m_frameCountToDisplay; }

private:

	// Delta time
	static double	m_deltaTime;
	static double	m_lastFrame;

	static double	m_previousTime;
	static int		m_frameCount;
	static int		m_frameCountToDisplay;
	static double	m_currentFrame;

	ApplicationClock();
};


/// <summary>
/// Simple timer used to measure performance by printing the time it takes between timer creation and destruction
/// Can be stopped manually, .stop(), or stops on scope end
/// </summary>
class PerformanceTimer
{
public:
	PerformanceTimer(const char* tag = "");
	~PerformanceTimer();

	void		stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;

	const char* m_tag;
	bool		m_stopped;
};

