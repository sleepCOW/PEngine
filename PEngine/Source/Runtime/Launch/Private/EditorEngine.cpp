#ifdef WITH_EDITOR
#include "Launch/Public/EditorEngine.h"

#include "Core/Public/CoreMinimal.h"
#include "Core/Public/ObjectManager.h"
#include "Core/Public/ReflectionManager.h"

#include "imgui/Public/imgui.h"
#include "imgui/Public/imgui_impl_sdl.h"
#include "imgui_sdl/Public/imgui_sdl.h"
#include "SDL2/Public/SDL.h"

#pragma init_seg(compiler)
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

void CEditorEngine::EditorUI(float DeltaTime)
{
	ImGui::NewFrame();

	ShowMenuBar();
	ImGui::ShowDemoWindow();

	if (bShowAddObject) ShowAddObject();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
}

void CEditorEngine::ShowMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void CEditorEngine::Test()
{
	ImGui::Begin("Select object to create");


	if (ImGui::Button("Create")) {}

	ImGui::End();
}

void CEditorEngine::ShowMenuFile()
{
	if (ImGui::MenuItem("New project")) {}
	if (ImGui::MenuItem("Open", "CTRL+O")) {}
	ImGui::Separator();
	if (ImGui::MenuItem("Create C++ Class")) {}
	if (ImGui::MenuItem("Add object on scene")) { bShowAddObject = true; }
}

void CEditorEngine::ShowAddObject()
{
	ImGui::Begin("Select object to create", &bShowAddObject);

	Vector<String>& AllObjects = ReflectionManager.GetObjects();
	assert(AllObjects.size()); // At least CObject should exist!
	
	/** #TODO: Ensure CObject always first in AllObjects list */
	static int Selected = 0;
	if (ImGui::BeginCombo("Object list", AllObjects[Selected].data()))
	{
		for (int i = 0; i < AllObjects.size(); ++i)
		{
			if (ImGui::Selectable(AllObjects[i].data()))
			{
				Selected = i;
			}
		}

		ImGui::EndCombo();
	}

	if (ImGui::Button("Create")) 
	{
		CObject* NewObj = ReflectionManager.CreateObject(AllObjects[Selected], nullptr);
		// Reset selected object
		Selected = 0;
	}

	//if (ImGui::Int)

	ImGui::End();
}

#endif