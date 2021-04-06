#ifdef WITH_EDITOR
#include "Launch/Public/EditorEngine.h"

#include "Core/Public/CoreMinimal.h"
#include "Core/Public/ObjectManager.h"
#include "Core/Public/ReflectionManager.h"

#include "imgui/Public/imgui.h"
#include "imgui/Public/imgui_impl_sdl.h"
#include "imgui_sdl/Public/imgui_sdl.h"
#include "SDL2/Public/SDL.h"

CReflectionManager ReflectionManager;

bool CEditorEngine::Init()
{
	Super::Init();

	assert(GMainWindow);
	assert(GRenderer);

	int Width, Height;
	SDL_GetWindowSize(GMainWindow, &Width, &Height);

	ImGui::CreateContext();
	ImGuiSDL::Initialize(GRenderer, Width, Height);

	return true;
}

void CEditorEngine::Close()
{
	Super::Close();

	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
}

void CEditorEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void CEditorEngine::EditorUI(float DeltaTime)
{
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
}

void CEditorEngine::HandleInput(const SDL_Event& Event)
{
	Super::HandleInput(Event);

	ImGuiIO& InputOutput = ImGui::GetIO();
	int Wheel = 0;

	ImGui_ImplSDL2_ProcessEvent(&Event);

	if (Event.type == SDL_WINDOWEVENT)
	{
		if (Event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			InputOutput.DisplaySize.x = static_cast<float>(Event.window.data1);
			InputOutput.DisplaySize.y = static_cast<float>(Event.window.data2);
		}	
	}
	else if (Event.type == SDL_MOUSEWHEEL)
	{
		Wheel = Event.wheel.y;
	}

	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	// Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)
	InputOutput.DeltaTime = 1.0f / 60.0f;
	InputOutput.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	InputOutput.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	InputOutput.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	InputOutput.MouseWheel = static_cast<float>(Wheel);
}
#endif