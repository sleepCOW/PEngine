#include "Core/Public/CoreMinimal.h"
#include "Launch/Public/EngineLoop.h"

#include <windows.h>

CEngineLoop* GEngineLoop = nullptr;

#include "SDL2/Public/SDL.h"
#include <iostream>
using namespace std;

bool init();
void kill();
bool loop();

// Pointers to our window and renderer
SDL_Window* window;
SDL_Renderer* renderer;

int WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char*, _In_ int nCmdShow)
{
#ifdef WITH_EDITOR
	GEngineLoop = new EditorEngine; bla bla bla
#else
	GEngineLoop = new CEngineLoop;
#endif

	return Run(GEngineLoop);
}

bool loop() {

	static const unsigned char* keys = SDL_GetKeyboardState(NULL);

	SDL_Event e;
	SDL_Rect r;
	// For mouse rectangle (static to presist between function calls)
	static int mx0 = -1, my0 = -1, mx1 = -1, my1 = -1;

	// Clear the window to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Event loop
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_QUIT:
			return false;
		case SDL_MOUSEBUTTONDOWN:
			mx0 = e.button.x;
			my0 = e.button.y;
			break;
		case SDL_MOUSEMOTION:
			mx1 = e.button.x;
			my1 = e.button.y;
			break;
		case SDL_MOUSEBUTTONUP:
			mx0 = my0 = mx1 = my1 = -1;
			break;
		}
	}

	// Set drawing color to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Test key states - this could also be done with events
	if (keys[SDL_SCANCODE_1]) {
		SDL_RenderDrawPoint(renderer, 10, 10);
	}
	if (keys[SDL_SCANCODE_2]) {
		SDL_RenderDrawLine(renderer, 10, 20, 10, 100);
	}
	if (keys[SDL_SCANCODE_3]) {
		r.x = 20;
		r.y = 20;
		r.w = 100;
		r.h = 100;
		SDL_RenderFillRect(renderer, &r);
	}

	// Render mouse rectangle
	if (mx0 != -1) {
		r.x = mx0;
		r.y = my0;
		r.w = mx1 - mx0;
		r.h = my1 - my0;
		SDL_RenderDrawRect(renderer, &r);
	}

	// Update window
	SDL_RenderPresent(renderer);

	return true;
}

bool init() {
	// See last example for comments
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	return true;
}

void kill() {
	// Quit
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}