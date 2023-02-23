#include <windows.h>

#include "Engine/Core/Engine.h"

// Keep console open after program end (debug mode)
#ifdef _DEBUG
	#define KeepConsoleOpen while (true) {}
#else
	#define KeepConsoleOpen 
#endif

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	StartEngine();

	KeepConsoleOpen;
	
	return 0;
}
