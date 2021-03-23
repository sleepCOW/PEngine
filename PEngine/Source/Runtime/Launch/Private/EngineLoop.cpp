#include "Launch/Public/EngineLoop.h"

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
	return true;
}

void CEngine::Close()
{

}

void CEngine::Tick(float DeltaTime)
{

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

			ImGuiIO& io = ImGui::GetIO();
			int wheel = 0;
			while (SDL_PollEvent(&Event) != 0) {
				ImGui_ImplSDL2_ProcessEvent(&Event);

				if (Event.type == SDL_WINDOWEVENT)
				{
					if (Event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						io.DisplaySize.x = static_cast<float>(Event.window.data1);
						io.DisplaySize.y = static_cast<float>(Event.window.data2);
					}
				}
				else if (Event.type == SDL_QUIT)
				{
					GIsRequestingExit = true;
				}
			}

			int mouseX, mouseY;
			const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

			// Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)

			io.DeltaTime = 1.0f / 60.0f;
			io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
			io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
			io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
			io.MouseWheel = static_cast<float>(wheel);

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

