#include "pch.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

/// <summary>
/// Sets the text colour and timestamp, name and log message settings
/// </summary>
void Log::init()
{
	//%^ colouring, [%T] timestamp, %n name of the logger, %v%$ actual log message
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_CoreLogger = spdlog::stdout_color_mt("OpenGL_Project");
	s_CoreLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Log::getCoreLogger()
{
	return s_CoreLogger;
}
