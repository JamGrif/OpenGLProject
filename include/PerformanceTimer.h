#pragma once

#include <chrono>

/// <summary>
/// Simple timer used to measure performance by printing the time it takes between timer creation and destruction
/// Can be stopped manually, .stop(), or stops on scope end
/// </summary>
class PerformanceTimer
{
public:
	PerformanceTimer(const char* tag = "")
		:m_tag(tag), m_stopped(false)
	{
		m_startTimepoint = std::chrono::high_resolution_clock::now();
	}
	~PerformanceTimer()
	{
		if (!m_stopped)
			stop();
	}

	void stop()
	{
		m_stopped = true;

		auto endTimepoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;

		if (m_tag != "")
		{
			std::cout << m_tag << " - Duration is " << ms << std::endl;
		}
		else
		{
			std::cout << "Duration is " << ms << std::endl;
		}
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;

	const char* m_tag;
	bool		m_stopped;
};
