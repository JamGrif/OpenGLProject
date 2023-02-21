
#include <iostream>
#include <windows.h>

#include "C:\Users\jamgr\Desktop\Stuff\dev\OpenGLProject\include\EntryPoint.h"

static constexpr int CONSOLE_STARTING_X = -900;
static constexpr int CONSOLE_STARTING_Y = 200;

static constexpr int CONSOLE_WIDTH = 900;
static constexpr int CONSOLE_HEIGHT = 500;

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	// Enable console and set initial position
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	MoveWindow(GetConsoleWindow(), CONSOLE_STARTING_X, CONSOLE_STARTING_Y, CONSOLE_WIDTH, CONSOLE_HEIGHT, TRUE);

	StartProgram();

	return 0;
}
