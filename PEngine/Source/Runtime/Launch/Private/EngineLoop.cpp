#include "Launch/Public/EngineLoop.h"

#include "Core/Public/ObjectManager.h"
#include "Core/Public/ReflectionManager.h"
#include "Component/Public/RenderComponent.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL2/Public/SDL.h"
#include "SDL2/Public/SDL_image.h"

#include "imgui/Public/imgui.h"
#include "imgui/Public/imgui_impl_sdl.h"
#include "imgui_sdl/Public/imgui_sdl.h"
#include "Object/Public/Level.h"
#include "Launch/Public/EditorEngine.h"

// Global variables definition
SDL_Renderer* GRenderer;
SDL_Window* GMainWindow;

/** sleepCOW: make sure ReflectionManager is created among first created objects */
#pragma init_seg(compiler)
CReflectionManager GReflectionManager;

CEngine::CEngine()
{
	CurrentLevel = nullptr;
	bGamePaused = false;
}

void CEngine::PreInit(SWindowParam& OutWindowParam)
{
	OutWindowParam.bFullscreen = false;
	OutWindowParam.Width = 1280;
	OutWindowParam.Height = 720;
	OutWindowParam.WindowTitle = "PEngine editor";
}

bool CEngine::Init()
{
	ObjectManager = std::make_unique<CObjectManager>();
	ConfReader = std::make_unique<CFileManager>();

	CurrentLevel.reset(CreateObject<CLevel>(nullptr));

	String Level = ConfReader->GetString("StartupLevel");
	if (!Level.empty())
	{
		CurrentLevel->LoadFromJson(Level);
	}
	else
	{
		CurrentLevel->NewLevel();
	}

	return true;
}

void CEngine::Close()
{

}

void CEngine::Tick(float DeltaTime)
{
	for (auto& Object : ObjectManager->GetObjects())
	{
#ifdef WITH_EDITOR
		// EditorTick is always enabled
		Object->EditorTick(DeltaTime);
#endif
			
		// If game not paused and object is marked tickable
		if (!bGamePaused && Object->bTicking)
		{
			Object->Tick(DeltaTime);
		}
	}

	// Render part
	for (auto& RenderComponent : ObjectManager->GetRenderComponents())
	{
		if (RenderComponent->bTicking)
		{
			RenderComponent->Draw();
		}
	}
}

void CEngine::HandleInput(const SDL_Event& Event)
{
	if (Event.type == SDL_QUIT)
	{
		GIsRequestingExit = true;
	}
}

CObjectManager* CEngine::GetObjectManager() const
{
	return ObjectManager.get();
}

CFileManager* CEngine::GetConfReader() const
{
	return ConfReader.get();
}

SDL_Renderer* CreateOpenGLRenderer(SDL_Window* Window)
{
	for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
	{
		SDL_RendererInfo rendererInfo = {};
		SDL_GetRenderDriverInfo(i, &rendererInfo);
		if (strcmp(rendererInfo.name, "opengl") != 0)
		{
			continue;
		}

		return SDL_CreateRenderer(Window, i, SDL_RENDERER_ACCELERATED);
	}

	return nullptr;
}

int Run(CEngine* EngineLoop)
{
	SDL_Event Event;
	SWindowParam WindowParam;
	uint32_t WindowFlags;

	EngineLoop->PreInit(WindowParam);

	WindowFlags = SDL_WINDOW_SHOWN;
	WindowFlags |= SDL_WINDOW_RESIZABLE;
	if (WindowParam.bFullscreen) {
		SDL_ShowCursor(0);
		WindowFlags |= SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		fprintf(stdout, "SDL_Init(SDL_INIT_VIDEO) failed: %s\n", SDL_GetError());
		return(2);
	}

	GMainWindow = SDL_CreateWindow(WindowParam.WindowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowParam.Width, WindowParam.Height, WindowFlags);
	if (!GMainWindow) {
		fprintf(stdout, "SDL_CreateWindow() failed: %s\n", SDL_GetError());
		return false;
	}

	GRenderer = CreateOpenGLRenderer(GMainWindow);
	if (!GRenderer) {
		fprintf(stdout, "SDL_CreateRenderer() failed: %s\n", SDL_GetError());
		return false;
	}

	{
		GEngineInitialized = true;

		if (!EngineLoop->Init())
		{
			fprintf(stdout, "EngineLoop::Init failed\n");
			SDL_Quit();
			exit(1);
		}

		uint32_t End = SDL_GetTicks();
		while (!IsEngineExitRequested()) { 
			uint32_t Start = SDL_GetTicks();
			float DeltaTime = (Start - End) / 1000.f;
			SDL_RenderClear(GRenderer);

			End = SDL_GetTicks();

			while (SDL_PollEvent(&Event) != 0) {
				EngineLoop->HandleInput(Event);
			}

			EngineLoop->Tick(DeltaTime);
#ifdef WITH_EDITOR
			static_cast<CEditorEngine*>(EngineLoop)->EditorUI(DeltaTime);
#endif

			// Reset background to black
			SDL_SetRenderDrawColor(GRenderer, 0, 0, 0, 0);

			SDL_RenderPresent(GRenderer);
			SDL_Delay(1);
		}
	}

	EngineLoop->Close();
	delete EngineLoop;

	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(GMainWindow);

	GEngineInitialized = false;

	/* We're done! */
	SDL_Quit();
	return(0);
}