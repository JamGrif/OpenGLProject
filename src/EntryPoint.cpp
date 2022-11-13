#include "pch.h"

#include "Core/Application.h"


int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	// Enable console
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	std::unique_ptr<Application> app = std::make_unique<Application>();
	
	if (app->appInit())
	{
		app->appLoop();
	}

	return 0;
}
