#include "Launch/Public/EditorEngine.h"
#include "Launch/Public/GameEngine.h"

#include <iostream>
#include <fcntl.h>
#include <corecrt_io.h>
#include <windows.h>

CEngine* GEngineLoop = nullptr;

// #TODO: Refactor
void RedirectIOToConsole()
{
	AllocConsole();
	HANDLE StdHandle;
	int ConsoleHandle;
	FILE* File;
	StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleHandle = _open_osfhandle((long)StdHandle, _O_TEXT);
	File = _fdopen(ConsoleHandle, "w");

	freopen_s(&File, "CONOUT$", "w", stdout);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}


#include <cstdio>

int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char*, _In_ int nCmdShow)
{
	RedirectIOToConsole();

#ifdef WITH_EDITOR
	GEngineLoop = new CEditorEngine;
#else
	GEngineLoop = new CGameEngine;
#endif

	int ErrorCode = Run(GEngineLoop);
	return ErrorCode;
}