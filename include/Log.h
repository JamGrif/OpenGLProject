#pragma once


#include "spdlog/spdlog.h"

class Log
{
public:
	static void init();

	static std::shared_ptr<spdlog::logger>& getCoreLogger();


private:

	static std::shared_ptr<spdlog::logger> s_CoreLogger;

};

// Logging Macros
#define PRINT_ERROR(...) ::Log::getCoreLogger()->warn(__VA_ARGS__)
