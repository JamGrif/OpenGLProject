#include "pch.h"
#include "EntryPoint.h"

#include "Core/Application.h"

/// <summary>
/// Entry point for application
/// </summary>
int StartProgram()
{
	std::unique_ptr<Application> app = std::make_unique<Application>();
	
	if (app->AppInit())
		app->AppLoop();

	return 0;
}
