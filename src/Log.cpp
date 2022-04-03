#include "pch.h"

#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;


void Log::init()
{
	//%^ colouring, [%T] timestamp, %n name of the logger, %v%$ actual log message
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_CoreLogger = spdlog::stdout_color_mt("Jamie");
	s_CoreLogger->set_level(spdlog::level::trace);

}

std::shared_ptr<spdlog::logger>& Log::getCoreLogger()
{
	return s_CoreLogger;
}
