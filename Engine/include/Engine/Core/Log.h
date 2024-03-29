#pragma once

#include "spdlog/logger.h"

/// <summary>
/// Logging system that prints text to the console
/// Provides four different printing types which set colour of text
/// </summary>
class Log
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger>& GetLogger() { return s_pLogger; }

private:

	static std::shared_ptr<spdlog::logger>	s_pLogger;

	Log();
};

// Logging Macros - only in debug mode
#ifdef _DEBUG
	#define PRINT_TRACE(...) ::Log::GetLogger()->trace(__VA_ARGS__)	// White
	#define PRINT_INFO(...) ::Log::GetLogger()->info(__VA_ARGS__)	// Green
	#define PRINT_WARN(...) ::Log::GetLogger()->warn(__VA_ARGS__)	// Yellow
	#define PRINT_ERROR(...) ::Log::GetLogger()->error(__VA_ARGS__) // Red
#else
	#define PRINT_TRACE(...)
	#define PRINT_INFO(...)
	#define PRINT_WARN(...) 
	#define PRINT_ERROR(...)
#endif
