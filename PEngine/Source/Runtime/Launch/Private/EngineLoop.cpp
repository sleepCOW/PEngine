#include "Launch/Public/EngineLoop.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "SDL2/Public/SDL.h"
#include "SDL2/Public/SDL_image.h"

static SDL_Renderer* GRenderer;

void CEngineLoop::PreInit(SWindowParam& OutWindowParam)
{
	OutWindowParam.bFullscreen = false;
	OutWindowParam.Width = 800;
	OutWindowParam.Height = 600;
	OutWindowParam.WindowTitle = "Test";
}

bool CEngineLoop::Init()
{
	return true;
}

void CEngineLoop::Close()
{

}

void CEngineLoop::Tick(float DeltaTime)
{

}

int Run(IEngineLoop* EngineLoop)
{
	SDL_Window* Window;
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

	Window = SDL_CreateWindow(WindowParam.WindowTitle.data(), 200, 200, WindowParam.Width, WindowParam.Height, flags);
	if (!Window) {
		fprintf(stderr, "SDL_CreateWindow() failed: %s\n", SDL_GetError());
		return false;
	}

	GRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	if (!GRenderer) {
		fprintf(stderr, "SDL_CreateRenderer() failed: %s\n", SDL_GetError());
		return false;
	}
	{
		GEngineInitialized = true;

		if (!EngineLoop->Init())
		{
			fprintf(stderr, "Framework::Init failed\n");
			SDL_Quit();
			exit(1);
		}

		uint32_t End = SDL_GetTicks();
		while (!IsEngineExitRequested()) { 
			uint32_t Start = SDL_GetTicks();
			float DeltaTime = (Start - End) / 1000.f;
			SDL_RenderClear(GRenderer);

			SDL_Rect Viewport;
			SDL_RenderGetViewport(GRenderer, &Viewport);

			while (SDL_PollEvent(&Event) != 0) {
				switch (Event.type)
				{
					case SDL_QUIT:
						GIsRequestingExit = true;
				}
			}

			EngineLoop->Tick(DeltaTime);

			SDL_RenderPresent(GRenderer);
			End = SDL_GetTicks();
			SDL_Delay(10);
		}
	}

	EngineLoop->Close();

	SDL_DestroyRenderer(GRenderer);
	SDL_DestroyWindow(Window);

	GEngineInitialized = false;

	/* We're done! */
	SDL_Quit();
	return(0);
}

