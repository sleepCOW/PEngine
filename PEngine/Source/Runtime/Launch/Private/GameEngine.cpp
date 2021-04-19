#include "Launch/Public/GameEngine.h"
#include "Object/Public/Level.h"

bool CGameEngine::Init()
{
	bool bSuccess = Super::Init();

	CurrentLevel->Init();

	return bSuccess;
}