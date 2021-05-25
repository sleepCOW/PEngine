#ifdef WITH_EDITOR
#include "Launch/Public/EditorEngine.h"

#include "Core/Public/CoreMinimal.h"
#include "Core/Public/ObjectManager.h"
#include "Object/Public/Level.h"

#include "imgui/Public/imgui.h"
#include "imgui/Public/imgui_impl_sdl.h"
#include "imgui/Public/imgui_stdlib.h"
#include "imgui_sdl/Public/imgui_sdl.h"
#include "SDL2/Public/SDL.h"
#include "Core/Public/Component.h"
#include "Core/Public/Actor.h"
#include "Component/Public/OutlineComponent.h"

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
	bLevelViewHovered = false;
	SelectedObject = nullptr;

	// Input
	bLMBDown = false;
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
	else if (Event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (Event.button.button == SDL_BUTTON_LEFT)
		{
			if (!bLevelViewHovered)
			{
				for (auto& Object : CurrentLevel->GetObjects())
				{
					CActor* Actor = dynamic_cast<CActor*>(Object);

					if (!Actor)
						continue;

					// Actor guaranteed to have OutlineComponent
					COutlineComponent* OutlineComp = Actor->GetComponentByClass<COutlineComponent>();
					
					if (SelectedObject != Actor && SDL_PointInRect(&MousePosition, &OutlineComp->GetRectangle()))
					{
						SelectedObject = Actor;
						break;
					}
				}
			}
			bLMBDown = true;
		}
	}
	else if (Event.type == SDL_MOUSEBUTTONUP)
	{
		if (Event.button.button == SDL_BUTTON_LEFT)
		{
			bLMBDown = false;
		}
	}

	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	MousePosition.x = mouseX;
	MousePosition.y = mouseY;

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

void CEditorEngine::SetSelectedObject(CObject* NewObject)
{
	SelectedObject = NewObject;
}

void CEditorEngine::ResetInputBuffer(String& Field)
{
	static CObject* PrevObject = nullptr;
	static String* PrevField = nullptr;
	if (PrevObject != SelectedObject ||
		PrevField != &Field)
	{
		InputBuffer = Field;
		PrevObject = SelectedObject;
		PrevField = &Field;
	}
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
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Level view")) { bShowLevelView = true; }
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

	Vector<String>& AllObjects = GReflectionManager.GetObjects();
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
		GReflectionManager.CreateActor(AllObjects[Selected], GEngine->GetLevel());
		// Reset selected object
	}

	ImGui::End();
}

void CEditorEngine::ShowAddComponent()
{
	ImGui::Begin("Add Component", &bShowAddComponent);

	Vector<String>& AllComponents = GReflectionManager.GetComponents();
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
		GReflectionManager.CreateObject(AllComponents[Selected], SelectedObject);
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
		if (ImGui::Button("Save Level", WindowWidth()))
		{
			CurrentLevel->SaveToJson();
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

	bLevelViewHovered = ImGui::IsWindowHovered();

	ShowPlayButtons();

	ImGui::LabelText(GetLevel()->GetName().data(), "Level name: ");

	ImGui::Text("Level view:");

	Vector<CObject*>& Objects = GEngine->GetLevel()->GetObjects();
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
	ImGui::Text(Field.FieldName);
	ImGui::PushID(&Field);
	if (Field.FieldType == EFieldType::MATH_VECTOR)
	{
		SVector& FVector = Field.Get<SVector>();

		if (ImGui::DragFloat("X", &FVector.X, FVector.DragSpeed))
		{
			SelectedObject->PostEditChangeProperty(Field);
		}
		if (ImGui::DragFloat("Y", &FVector.Y, FVector.DragSpeed))
		{
			SelectedObject->PostEditChangeProperty(Field);
		}
	}
	else if (Field.FieldType == EFieldType::STRING)
	{
		String& FStringField = Field.Get<String>();

		// If we changed selected object or field then reset input buffer to prevent previous changes applied to a new field!
		ResetInputBuffer(FStringField);

		if (ImGui::InputText(Field.FieldName, &InputBuffer))
		{
			FStringField = InputBuffer;

			SelectedObject->PostEditChangeProperty(Field);
		}
	}
	else if (Field.FieldType == EFieldType::RECTANGLE)
	{
		SDL_Rect& Rect = Field.Get<SDL_Rect>();

		if (ImGui::DragInt("Width", &Rect.w, 1.f, 0, 4000))
		{
			SelectedObject->PostEditChangeProperty(Field);
		}
		if (ImGui::DragInt("Height", &Rect.h, 1.f, 0, 4000))
		{
			SelectedObject->PostEditChangeProperty(Field);
		}
	}
	ImGui::PopID();
	ImGui::Separator();
}

#endif