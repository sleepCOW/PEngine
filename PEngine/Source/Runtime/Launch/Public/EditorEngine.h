#ifdef WITH_EDITOR
#pragma once

#include "Launch/Public/EngineLoop.h"

/**
 * Implements editor engine (have addional UI layer)
 */
class CEditorEngine : public CEngine 
{
	DECLARE_CLASS(CEditorEngine, CEngine)

public:
	CEditorEngine();

	virtual bool Init() override;

	virtual void Close() override;

	virtual void Tick(float DeltaTime) override;

	void EditorUI(float DeltaTime);

	virtual void HandleInput(const SDL_Event& Event) override;

protected:

	void PlayInEditor();
	void StopPIE();

protected:

	CObject* SelectedObject;

	// UI BEGIN

	void ShowMenuBar();
	void ShowMenuFile();

	void ShowAddObject();
	void ShowAddComponent();
	void ShowLevelView();
	void ShowCreateClass();
	void ShowObjectEdit();
	void ShowField(SField& Field);
	void ShowPlayButtons();

	bool bShowAddObject;
	bool bShowAddComponent;
	bool bShowCreateClass;
	bool bShowLevelView;

	// UI END
};
#endif