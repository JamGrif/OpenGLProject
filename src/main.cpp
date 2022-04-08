#include "pch.h"

#include "Core/Application.h"

int main()
{
	std::unique_ptr<Application> app = std::make_unique<Application>();
	
	if (app->appInit())
	{
		app->appLoop();
	}

	return 0;
}
