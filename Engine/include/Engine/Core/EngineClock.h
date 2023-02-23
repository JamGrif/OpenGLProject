#pragma once

#include <chrono>

/// <summary>
/// Global timer for the application
/// Ticked every frame and can return application's delta time
/// </summary>
class EngineClock
{
public:

	void		Init();
	void		Tick();

	double		GetDeltaTime() { return m_deltaTime; }
	int			GetFrameCount() { return m_frameCountToDisplay; }

	static EngineClock* Get() // Get instance
	{
		static EngineClock* s_pInstance = new EngineClock;
		return s_pInstance;
	}

private:

	double		m_deltaTime;
	double		m_lastFrame;

	double		m_previousTime;
	uint16_t	m_frameCount;
	uint16_t	m_frameCountToDisplay;
	double		m_currentFrame;

	EngineClock() :
		m_deltaTime(0.0), m_lastFrame(0.0), m_previousTime(0.0),
		m_frameCount(0), m_frameCountToDisplay(0), m_currentFrame(0.0) {}
	~EngineClock() {}
	EngineClock(const EngineClock&) = delete;
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

	void		Stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;

	const char* m_tag;
	bool		m_bStopped;
};
