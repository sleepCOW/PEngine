#include "Launch/Public/EditorEngine.h"
#include "Launch/Public/GameEngine.h"

#include <windows.h>

CEngine* GEngineLoop = nullptr;

int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char*, _In_ int nCmdShow)
{
#ifdef WITH_EDITOR
	GEngineLoop = new CEditorEngine;
#else
	GEngineLoop = new CGameEngine;
#endif

	int ErrorCode = Run(GEngineLoop);
	system("pause");
	return ErrorCode;
}