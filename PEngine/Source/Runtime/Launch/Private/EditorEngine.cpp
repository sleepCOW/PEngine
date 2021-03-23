#include "Launch/Public/EditorEngine.h"

#include "imgui/Public/imgui.h"
#include "imgui_sdl/Public/imgui_sdl.h"
#include "SDL2/Public/SDL.h"

bool CEditorEngine::Init()
{
	SDL_assert(GMainWindow);

	int Width, Height;
	SDL_GetWindowSize(GMainWindow, &Width, &Height);

	ImGui::CreateContext();
	ImGuiSDL::Initialize(GRenderer, Width, Height);

	return true;
}

void CEditorEngine::Close()
{
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
}

void CEditorEngine::Tick(float DeltaTime)
{

}

void CEditorEngine::EditorUI(float DeltaTime)
{
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
}