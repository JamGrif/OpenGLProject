#include "Application.h"

int main()
{
	
	Application* app = new Application();

	if (app->appInit() == 0)
	{
		app->appLoop();
	}

	delete app;
	app = nullptr;

	return 0;
}