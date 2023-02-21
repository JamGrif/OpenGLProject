#include "pch.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_pLogger;

/// <summary>
/// Sets the text colour, timestamp, name and log message settings
/// </summary>
void Log::Init()
{
	//%^ colouring, [%T] timestamp, %n name of the logger, %v%$ actual log message
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_pLogger = spdlog::stdout_color_mt("OpenGL_Project");
	s_pLogger->set_level(spdlog::level::trace);
}
