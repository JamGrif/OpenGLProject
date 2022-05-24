#pragma once

#include "spdlog/spdlog.h"

class Log
{
public:
	static void init();

	static std::shared_ptr<spdlog::logger>& getCoreLogger();

private:

	static std::shared_ptr<spdlog::logger>	s_CoreLogger;

	Log();
};

// Logging Macros
#define PRINT_TRACE(...) ::Log::getCoreLogger()->trace(__VA_ARGS__)	// White
#define PRINT_INFO(...) ::Log::getCoreLogger()->info(__VA_ARGS__)	// Green
#define PRINT_WARN(...) ::Log::getCoreLogger()->warn(__VA_ARGS__)	// Yellow
#define PRINT_ERROR(...) ::Log::getCoreLogger()->error(__VA_ARGS__) // Red
