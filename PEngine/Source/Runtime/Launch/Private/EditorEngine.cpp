#ifdef WITH_EDITOR
#include "Launch/Public/EditorEngine.h"

#include "Core/Public/CoreMinimal.h"
#include "Core/Public/ObjectManager.h"
#include "Core/Public/ReflectionManager.h"
#include "Object/Public/Level.h"

#include "imgui/Public/imgui.h"
#include "imgui/Public/imgui_impl_sdl.h"
#include "imgui/Public/imgui_stdlib.h"
#include "imgui_sdl/Public/imgui_sdl.h"
#include "SDL2/Public/SDL.h"
#include "Core/Public/Component.h"

/** sleepCOW: make sure ReflectionManager is created among first created objects */
#pragma init_seg(compiler)
CReflectionManager ReflectionManager;

static ImVec2 WindowWidth()
{
	return ImVec2(ImGui::GetWindowWidth() * 0.96f, 0.f);
}

CEditorEngine::CEditorEngine()
{
	bShowAddObject = false;
	bShowCreateClass = false;
	bShowAddComponent = false;
	bShowLevelView = true;
	bGamePaused = true;
	SelectedObject = nullptr;
}

bool CEditorEngine::Init()
{
	Super::Init();

	assert(GMainWindow);
	assert(GRenderer);

	int Width, Height;
	SDL_GetWindowSize(GMainWindow, &Width, &Height);

	ImGui::CreateContext();
	ImGuiSDL::Initialize(GRenderer, Width, Height);

	CurrentLevel->bTicking = false;

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
	InputOutput.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	InputOutput.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	InputOutput.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	InputOutput.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
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
	//InputOutput.DeltaTime = 1.0f / 120.0f;
	InputOutput.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	InputOutput.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	InputOutput.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	InputOutput.MouseWheel = static_cast<float>(Wheel);
}

void CEditorEngine::EditorUI(float DeltaTime)
{
	ImGui::NewFrame();

	ShowMenuBar();
	
	if (bShowAddObject) ShowAddObject();
	if (bShowLevelView) ShowLevelView();
	if (bShowCreateClass) ShowCreateClass();
	if (bShowAddComponent) ShowAddComponent();

	/** #TODO: Remove demo window */
	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
}

void CEditorEngine::PlayInEditor()
{
	CurrentLevel->Init();
	CurrentLevel->bTicking = true;
	SetGamePaused(false);
}

void CEditorEngine::StopPIE()
{
	// #TODO
	CurrentLevel->bTicking = false;
	SetGamePaused(true);
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

void CEditorEngine::ShowMenuFile()
{
	if (ImGui::MenuItem("New project")) {}
	if (ImGui::MenuItem("Open", "CTRL+O")) {}
	ImGui::Separator();
	if (ImGui::MenuItem("Create C++ Class")) { bShowCreateClass = true; }
	if (ImGui::MenuItem("Add object on scene")) { bShowAddObject = true; }
}

void CEditorEngine::ShowAddObject()
{
	ImGui::Begin("Add object to the level", &bShowAddObject);

	Vector<String>& AllObjects = ReflectionManager.GetObjects();
	assert(AllObjects.size()); // At least CObject should exist!
	
	/** #TODO sleepCOW: Ensure CObject always first in AllObjects list */
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

	if (ImGui::Button("Create", WindowWidth())) 
	{
		ReflectionManager.CreateActor(AllObjects[Selected], GEngineLoop->GetLevel());
		// Reset selected object
	}

	ImGui::End();
}

void CEditorEngine::ShowAddComponent()
{
	ImGui::Begin("Add Component", &bShowAddComponent);

	Vector<String>& AllComponents = ReflectionManager.GetComponents();
	assert(AllComponents.size()); // At least CObject should exist!

	static int Selected = 0;
	if (ImGui::BeginCombo("Component list", AllComponents[Selected].data()))
	{
		for (int i = 0; i < AllComponents.size(); ++i)
		{
			if (ImGui::Selectable(AllComponents[i].data()))
			{
				Selected = i;
			}
		}

		ImGui::EndCombo();
	}

	if (ImGui::Button("Create", WindowWidth()))
	{
		ReflectionManager.CreateObject(AllComponents[Selected], SelectedObject);
	}

	ImGui::End();
}

void CEditorEngine::ShowPlayButtons()
{
	if (!CurrentLevel->bTicking)
	{
		if (ImGui::Button("Play in editor", WindowWidth()))
		{
			PlayInEditor();
		}
	}
	else
	{
		if (ImGui::Button(GetGamePaused() ? "Resume" : "Pause", WindowWidth() * 0.5f))
		{
			SetGamePaused(!GetGamePaused());
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop", WindowWidth() * 0.5f))
		{
			StopPIE();
		}
	}
}

void CEditorEngine::ShowLevelView()
{
	ImGui::Begin("Level view", &bShowLevelView);

	ShowPlayButtons();

	ImGui::LabelText(GetLevel()->GetName().data(), "Level name: ");

	ImGui::Text("Level view:");

	Vector<CObject*>& Objects = GEngineLoop->GetLevel()->GetObjects();
	for (int i = 0; i < Objects.size(); ++i)
	{
		CObject* Object = Objects[i];
		ImGui::PushID(i);
		if (ImGui::SmallButton("Select"))
		{
			SelectedObject = Object;
		}
		ImGui::SameLine();
		if (ImGui::TreeNode((void*)(intptr_t)i, "%s Class: %s", Object->GetObjectName().data(), Object->GetClassName()))
		{
			Vector<CComponent*>& Components = Object->GetComponents();
			for (int k = 0; k < Components.size(); ++k)
			{
				CComponent* Component = Components[k];

				ImGui::PushID(k);
				if (ImGui::SmallButton("Select")) 
				{
					SelectedObject = Component;
				}
				ImGui::SameLine();
				ImGui::Text("%s Class: %s", Component->GetObjectName().data(), Component->GetClassName());
				ImGui::PopID();
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	ImGui::Separator();
	
	if (SelectedObject) { ShowObjectEdit(); }

	ImGui::End();
}

void CEditorEngine::ShowCreateClass()
{
	ImGui::Begin("Create C++ Class", &bShowCreateClass);

	// #TODO sleepCOW

	ImGui::End();
}

void CEditorEngine::ShowObjectEdit()
{
	assert(SelectedObject);
	ImGui::Text("Object edit:");

	for (SField& Field : SelectedObject->GetEditorFields())
	{
		ShowField(Field);
	}

	if (ImGui::Button("Add Component", WindowWidth()))
	{
		bShowAddComponent = true;
	}
}

void CEditorEngine::ShowField(SField& Field)
{
	if (Field.FieldType == EFieldType::MATH_VECTOR)
	{
		SVector* FVector = reinterpret_cast<SVector*>(Field.PField);
		ImGui::DragFloat("X", &FVector->X);
		ImGui::DragFloat("Y", &FVector->Y);
	}
	else if (Field.FieldType == EFieldType::STRING)
	{
		String* FString = reinterpret_cast<String*>(Field.PField);

		if (ImGui::InputText("String field", FString))
		{

		}
	}
}

#endif