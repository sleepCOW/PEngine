#include "Launch/Public/EngineLoop.h"

#include "Core/Public/ObjectManager.h"
#include "Component/Public/RenderComponent.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL2/Public/SDL.h"
#include "SDL2/Public/SDL_image.h"

#include "imgui/Public/imgui.h"
#include "imgui/Public/imgui_impl_sdl.h"
#include "imgui_sdl/Public/imgui_sdl.h"

// Global variables definition
SDL_Renderer* GRenderer;
SDL_Window* GMainWindow;

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

	return true;
}

void CEngine::Close()
{

}

void CEngine::Tick(float DeltaTime)
{
	for (auto& Object : ObjectManager->GetObjects())
	{
		if (Object->bActive)
		{
			Object->Tick(DeltaTime);
		}
	}

	// Render part
	for (auto& RenderComponent : ObjectManager->GetRenderComponents())
	{
		if (RenderComponent->bActive)
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
	uint32_t flags;

	EngineLoop->PreInit(WindowParam);

	flags = SDL_WINDOW_SHOWN;
	if (WindowParam.bFullscreen) {
		SDL_ShowCursor(0);
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		fprintf(stderr, "SDL_Init(SDL_INIT_VIDEO) failed: %s\n", SDL_GetError());
		return(2);
	}

	GMainWindow = SDL_CreateWindow(WindowParam.WindowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowParam.Width, WindowParam.Height, flags);
	if (!GMainWindow) {
		fprintf(stderr, "SDL_CreateWindow() failed: %s\n", SDL_GetError());
		return false;
	}

	GRenderer = CreateOpenGLRenderer(GMainWindow);
	if (!GRenderer) {
		fprintf(stderr, "SDL_CreateRenderer() failed: %s\n", SDL_GetError());
		return false;
	}

	{
		GEngineInitialized = true;

		if (!EngineLoop->Init())
		{
			fprintf(stderr, "EngineLoop::Init failed\n");
			SDL_Quit();
			exit(1);
		}

		uint32_t End = SDL_GetTicks();
		while (!IsEngineExitRequested()) { 
			uint32_t Start = SDL_GetTicks();
			float DeltaTime = (Start - End) / 1000.f;
			SDL_RenderClear(GRenderer);

			while (SDL_PollEvent(&Event) != 0) {
				EngineLoop->HandleInput(Event);
			}

			EngineLoop->Tick(DeltaTime);
			EngineLoop->EditorUI(DeltaTime);

			SDL_RenderPresent(GRenderer);
			End = SDL_GetTicks();
			SDL_Delay(1);
		}
	}

	EngineLoop->Close();

	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(GMainWindow);

	GEngineInitialized = false;

	/* We're done! */
	SDL_Quit();
	return(0);
}