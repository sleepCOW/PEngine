#include "Core/Public/Object.h"

CObject::CObject(CObject* ThisOwner)
{
	if (ThisOwner)
	{
		Owner = ThisOwner->GetShared();
	}
}

CObject::~CObject()
{

}

void CObject::PreInit()
{

}

void CObject::Init()
{

}

void CObject::Tick(float DeltaTime)
{

}

SPtr<CObject> CObject::GetShared()
{
	return shared_from_this();
}

SPtr<const CObject> CObject::GetShared() const
{
	return shared_from_this();
}