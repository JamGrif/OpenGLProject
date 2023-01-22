#pragma once

#include "spdlog/logger.h"

class Log
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger>& GetLogger() { return s_pLogger; }

private:

	static std::shared_ptr<spdlog::logger>	s_pLogger;

	Log();
};

// Logging Macros
#define PRINT_TRACE(...) ::Log::GetLogger()->trace(__VA_ARGS__)	// White
#define PRINT_INFO(...) ::Log::GetLogger()->info(__VA_ARGS__)	// Green
#define PRINT_WARN(...) ::Log::GetLogger()->warn(__VA_ARGS__)	// Yellow
#define PRINT_ERROR(...) ::Log::GetLogger()->error(__VA_ARGS__) // Red
