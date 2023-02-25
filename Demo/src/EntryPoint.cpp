#include <windows.h>

#include "Engine/Core/Engine.h"

#include <string>

// Keep console open after program end (debug mode)
#ifdef _DEBUG
	#define KeepConsoleOpen() while (true) {}
#else
	#define KeepConsoleOpen() 
#endif

int wWinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	std::wstring wideString = pCmdLine;
	std::string processArgument;
	processArgument.resize(wideString.length());
	wcstombs_s(NULL, &processArgument[0], processArgument.size() + 1, wideString.c_str(), wideString.size());

	Engine::SetInitalScene(processArgument);
	Engine::StartEngine();

	//KeepConsoleOpen();
	
	return 0;
}
